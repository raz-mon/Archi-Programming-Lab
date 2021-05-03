#include <linux/limits.h>
#include "./LineParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

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
    int a = 1;
    if (waitpid((*process_list)->pid, &a, WNOHANG) == -1)
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
    }
    else if(strcmp(pCmdLine->arguments[0], "proc") == 0){
        if (processList == NULL)
            printf("nothing to print, bitch\n");
        else
            printProcessList(&processList);
    }
    else if(strcmp(pCmdLine->arguments[0], "free") == 0){
        if (processList == NULL)
            printf("nothing to free, bitch\n");
        else
            freeProcessList(processList);
    }
    else{
        int i = fork();
        if (pCmdLine->blocking == 1){
            int a = 1;
            waitpid(i, &a, 0);
        }
        if (i == 0){            
            int a = execvp(real_path, pCmdLine->arguments);
            if (a < 0 ){
                freeCmdLines(pCmdLine);
                perror("Could not execute the command!!");
                exit(1);
            }
        }
        addProcess(&processList, pCmdLine, i);
        //exit(0);            // No need for this process any more;
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

void updateProcessStatus(process* process_list, int pid, int status){
    if (process_list == NULL)
        return;
    if (process_list->pid == pid){
        process_list->status = status;
    }
    else if(process_list->next != NULL)
        updateProcessStatus(process_list->next, pid, status);
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