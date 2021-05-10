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
#define arrSize 10
typedef struct process{
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	                  /* next process in chain */
} process;
void print_hist();
int debug_mode;
process* processList;
char* history_arr[10];
int hist_arr_size = 0;
void addHist(char* line);
void freeHist();

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

char* getCommand(int command_ind){
    return history_arr[command_ind];
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

int contains_pipe(cmdLine *pCmdLine){
    if (pCmdLine->next != NULL)
        return 1;
    return -1;
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
        if (processList == NULL){
            printf("List empty\n");
            freeCmdLines(pCmdLine);
        }
        else{
            printProcessList(&processList);
            freeCmdLines(pCmdLine);
        }
    }
    else if(strcmp(pCmdLine->arguments[0], "free") == 0){
        if (processList == NULL){
            printf("List empty\n");
            freeCmdLines(pCmdLine);
        }
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
    else if(pCmdLine->arguments[0][0] == '!'){
        int command_ind = pCmdLine->arguments[0][1] - 48;

        if (command_ind > 9 || command_ind >= hist_arr_size){
            fprintf(stderr, "bad command index!");
            return;
        }

        cmdLine* pCmdLine_2 = parseCmdLines(getCommand(command_ind));
        addHist(getCommand(command_ind));
/*
        if (strcmp(pCmdLine->arguments[0], "quit") == 0){
        freeProcessList(processList);
        freeCmdLines(pCmdLine);
        freeHist();
        exit(0);
        }
*/  
        execute(pCmdLine_2);
        freeCmdLines(pCmdLine);
        freeCmdLines(pCmdLine_2);
    }
    else if(strcmp(pCmdLine->arguments[0], "history") == 0){
        print_hist();
        freeCmdLines(pCmdLine);        
    }
    else if (contains_pipe(pCmdLine) > 0) {

        cmdLine* nextCommand = pCmdLine->next;
        pCmdLine->next = NULL;

        int pipefd[2];
        pipe(pipefd);
        int pid = fork();
        if (pid == 0){                              // Child_1 process.
            close(STDOUT_FILENO);
            dup(pipefd[1]);
            close(pipefd[1]);
            // execute "ls -l".
            execute(pCmdLine);
            freeCmdLines(pCmdLine);
            freeProcessList(processList);
            freeHist();
            exit(0);
            /*
            if (execvp(pCmdLine->arguments[0], pCmdLine->arguments) < 0){
                fprintf(stderr, "executing ls has failed.\n");
                exit(0);
            }
            */
        }
        else{                                       // Main process.
            close(pipefd[1]);
            int pid_2 = fork();
            if (pid_2 == 0){                        // Child_2 process.
                close(STDIN_FILENO);
                dup(pipefd[0]);
                close(pipefd[0]);
                // execute "tail -n 2".
                execute(nextCommand);
                freeCmdLines(nextCommand);
                freeProcessList(processList);
                freeCmdLines(pCmdLine);
                freeHist();
                exit(0);
                /*
                if (execvp(pCmdLine->next->arguments[0], pCmdLine->next->arguments) < 0){
                    fprintf(stderr, "executing tail has failed.\n");
                    exit(0);
                }
                */
            }
            else{                                   // Main process.
                int a, b;
                close(pipefd[0]);
                waitpid(pid, &a, 0777);
                waitpid(pid_2, &b, 0777);       
                freeCmdLines(pCmdLine); 
                freeCmdLines(nextCommand);
            }      
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
        int i = fork();

        if (pCmdLine->blocking == 1){
            int a = 1;
            waitpid(i, &a, 0);
        }
        if (i == 0){      

            if (pCmdLine->inputRedirect != NULL | pCmdLine->outputRedirect != NULL){
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
}

void print_hist(){
    int flag;
    if (hist_arr_size > 9)
        flag = 10;
    else
        flag = hist_arr_size;
    for (int i = 0; i < flag; i++){
        printf("%s", history_arr[i]);
        
    }
}


void freeHist(){
    int flag;
    if (hist_arr_size > 9)
        flag = 10;
    else
        flag = hist_arr_size;
    for (int i = 0; i < flag; i++){
        free(history_arr[i]);
    }
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