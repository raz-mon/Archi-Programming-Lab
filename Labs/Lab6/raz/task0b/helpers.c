#include <linux/limits.h>
#include "./LineParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#define TERMINATED  -1
#define RUNNING 1
#define SUSPENDED 0
typedef struct process{
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	                  /* next process in chain */
} process;
int debug_mode;
process* processList;

char* toStatus(int status){
    switch(status)
    {
        case(1):
        return "RUNNING";
        break;
        case(0):
        return "SUSPENDED";
        break;
        case(-1):
        return "TERMINATED";
        break;
    }
}

void freeProcessList(process* process_list){
    freeCmdLines(process_list->cmd);
    if (process_list->next != NULL){
        freeProcessList(process_list->next);
        free(process_list);
    }
    else{
        free(process_list);
        processList = NULL;
    }
}

void updateProcessList(process **process_list){
    
    int status;
    pid_t result = waitpid((*process_list)->pid, &status, WNOHANG | WUNTRACED | WCONTINUED);

    if(result > 0) { // Signal has been received
        if (WIFSTOPPED(status)) {
            (*process_list)->status = SUSPENDED;
        } else if (WIFCONTINUED(status)) {
            (*process_list)->status = RUNNING;
        }
    }
    else if (result == -1)
        (*process_list)->status = TERMINATED;    
    
    if ((*process_list)->next != NULL)
        updateProcessList(&((*process_list)->next));
}

void printProcessList_2(process** process_list, int ind){
    
    printf("%d       %d      %s      %s\n", ind, (*process_list)->pid, (*process_list)->cmd->arguments[0], toStatus((*process_list)->status));
    if ((*process_list)->next != NULL){
        if ((*process_list)->status == -1){     // "freshly deleted".
            process* to_free = *process_list;
            freeCmdLines((*process_list)->cmd);
            free(to_free);
            *process_list = (*process_list)->next;
            printProcessList_2(process_list, ind + 1);
        }
        else{
            printProcessList_2(&((*process_list)->next), ind+1);        // process is not "freshly deleted".
        }
    }
    else if ((*process_list)->status == -1){     // "freshly deleted" -> Delete from list.
        freeCmdLines((*process_list)->cmd);
        processList = NULL;         // Similar to: *process_list = NULL;
    }
}       

void printProcessList(process** process_list){
    updateProcessList(process_list);
    if (*process_list != NULL){
        printf("ind     PID     Command     Status\n");
        printf("1       %d      %s      %s\n", (*process_list)->pid, (*process_list)->cmd->arguments[0], toStatus((*process_list)->status));
    }
    if ((*process_list)->next != NULL){
        if ((*process_list)->status == -1){     // "freshly deleted".
            process* to_free = *process_list;
            freeCmdLines((*process_list)->cmd);
            free(to_free);        
            *process_list = (*process_list)->next;
            printProcessList_2(process_list, 2);
        }
        else
            printProcessList_2(&((*process_list)->next), 2);        // First process is not "freshly deleted".
    }
    else if ((*process_list)->status == -1){     // "freshly deleted" -> Delete from list.
        freeCmdLines((*process_list)->cmd);
        processList = NULL;         // Similar to: *process_list = NULL;
    }
}

void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
    process* new_proc = (process*) malloc(sizeof(process));
    new_proc->cmd = cmd;
    new_proc->pid = pid;
    new_proc->status = RUNNING;
    if (process_list == NULL){
        new_proc->next = NULL;
    }
    else{
        new_proc->next = *process_list;
    }
    processList = new_proc;
}

int contains_pid(process* process_list, int pid){
    if (process_list->pid == pid)
        return pid;
    else if (process_list->next != NULL)
        return contains_pid(process_list->next, pid);
    else
        return -1;
}

void updateProcessStatus(process* process_list, int pid, int status){
    if (process_list == NULL)
        return;
    if (process_list->pid == pid){
        process_list->status = status;
    }
    else if(process_list->next != NULL)
        updateProcessStatus(process_list->next, pid, status);
}

void suspend(cmdLine * pCmdLine){
    int i = fork();
    pid_t pid = atoi(pCmdLine->arguments[1]);
    if(i == 0){
        if(pCmdLine->argCount != 3 || contains_pid(processList , pid)  == -1){
            perror("Fault: wrong arguments");
            return;
        }else{
            int cond = kill(pid, SIGTSTP);
            if(cond != 0){ perror("Cannot complete command"); return; }
            //updateProcessStatus(processList, pid, SUSPENDED);
            sleep(atoi(pCmdLine->arguments[2]));
            kill(pid, SIGCONT);
            if(cond != 0){ perror("Cannot complete command"); return; }
           // updateProcessStatus(processList, pid, RUNNING);
           freeProcessList(processList);
           exit(0);
        }
    }
}

void redirect(cmdLine* pCmdLine){
    if (pCmdLine->inputRedirect != NULL){
        close(STDIN_FILENO);
        fopen(pCmdLine->inputRedirect, "r");             // This fd will take the place of stdin.
    }
    if (pCmdLine->outputRedirect != NULL){
        close(STDOUT_FILENO);
        fopen(pCmdLine->outputRedirect, "w");             // This fd will take the place of stdin.
    }
}

void execute(cmdLine* pCmdLine){        // In this execute, the 'cd', 'proc' command are done from the main-process.
    char path[PATH_MAX];
    getcwd(path, PATH_MAX);
    char* real_path = strcat(path, "/");
    strcat(path, pCmdLine->arguments[0]);
    
    if (strcmp(pCmdLine->arguments[0], "cd") == 0){
        char* newPath = pCmdLine->arguments[1];
        int a = chdir(newPath);
        if (a == -1){
            perror("couldn't move to new path due to the error: ");
        }
        char path2[PATH_MAX];
        getcwd(path2, PATH_MAX);
        printf("current directory: %s\n", path2);       // Print the current directory.
        freeCmdLines(pCmdLine);
    }
    else if(strcmp(pCmdLine->arguments[0], "proc") == 0){
        if (processList == NULL)
            printf("List empty\n");
        else{
            printProcessList(&processList);
            freeCmdLines(pCmdLine);
        }
    }
    else if(strcmp(pCmdLine->arguments[0], "free") == 0){
        if (processList == NULL)
            printf("List empty\n");
        else{
            freeProcessList(processList);
            freeCmdLines(pCmdLine);
        }
    }
    else if(strcmp(pCmdLine->arguments[0], "suspend") == 0){
        if (contains_pid(processList, atoi(pCmdLine->arguments[1])) != -1){
            suspend(pCmdLine);
            freeCmdLines(pCmdLine);
        }
    }
    else if(strcmp(pCmdLine->arguments[0], "kill") == 0){
        if (contains_pid(processList, atoi(pCmdLine->arguments[1])) != -1){
            pid_t a = (pid_t)atoi(pCmdLine->arguments[1]);
            updateProcessStatus(processList, a, TERMINATED);
            int cond = kill(a, SIGINT);
            freeCmdLines(pCmdLine);
        }
    }
    /*
    else if(strcmp(pCmdLine->arguments[0], "wake") == 0){
        if (contains_pid(processList, atoi(pCmdLine->arguments[1])) != -1){
            pid_t a = (pid_t)pCmdLine->arguments[1];
            updateProcessStatus(processList, a, RUNNING);
            kill(a, SIGCONT);
        }
    }
    */
    else{
        /*
        int pipe_fd[2];
        if (pCmdLine->inputRedirect != NULL | pCmdLine->inputRedirect != NULL){
            if (pipe(pipe_fd) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            } 
        }
        */
        int i = fork();

        if (pCmdLine->blocking == 1){
            int a = 1;
            waitpid(i, &a, 0);
        }
        if (i == 0){      

            if (pCmdLine->inputRedirect != NULL | pCmdLine->inputRedirect != NULL){
                redirect(pCmdLine);
            }

            int a = execvp(pCmdLine->arguments[0], pCmdLine->arguments);
            if (a < 0 ){
                freeCmdLines(pCmdLine);
                perror("Could not execute the command!!");
                exit(1);
            }
        }
        addProcess(&processList, pCmdLine, i);
    }
        
    if (debug_mode == 1){
        fprintf(stderr, "PID: %d\n", getpid());
        fprintf(stderr, "Excecuting command: ");
        for (int i = 0; i < pCmdLine->argCount; ++i){
            fprintf(stderr, "%s ", pCmdLine->arguments[i]);    
        }
        fprintf(stderr, "\n");
    }
    //freeCmdLines(pCmdLine);
}














/*
void execute(cmdLine* pCmdLine){            // In this execute, the 'cd', 'proc' command are done from the child-process.
    char path[PATH_MAX];
    getcwd(path, PATH_MAX);
    char* real_path = strcat(path, "/");
    strcat(path, pCmdLine->arguments[0]);
    
        int i = fork();
        // addProcess(&processList, pCmdLine, i);
        if (pCmdLine->blocking == 1){
            int a = 1;
            waitpid(i, &a, 0);
        }
        if (i == 0){
            if (strcmp(pCmdLine->arguments[0], "cd") == 0){
                char* newPath = pCmdLine->arguments[1];
                int a = chdir(newPath);
                if (a == -1){
                    perror("couldn't move to new path due to the error: ");
                }
                char path2[PATH_MAX];
                getcwd(path2, PATH_MAX);
                printf("current directory: %s\n", path2);       // Print the current directory.
            }
            else if(strcmp(pCmdLine->arguments[0], "proc") == 0){
                printProcessList(&processList);
            }
            else{
                int a = execvp(real_path, pCmdLine->arguments);
                if (a < 0 ){
                    freeCmdLines(pCmdLine);
                    perror("Could not execute the command!!");
                    exit(1);
                }
            }
            exit(0);            // No need for this process any more;
        }
    if (debug_mode == 1){
        fprintf(stderr, "PID: %d\n", getpid());
        fprintf(stderr, "Excecuting command: ");
        for (int i = 0; i < pCmdLine->argCount; ++i){
            fprintf(stderr, "%s ", pCmdLine->arguments[i]);    
        }
        fprintf(stderr, "\n");
    }
    freeCmdLines(pCmdLine);
}

*/