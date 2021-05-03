#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "LineParser.h"
#include <string.h>
#include <wait.h>

#define TERMINATED  -1
#define RUNNING 1
#define SUSPENDED 0

void execute(cmdLine * pCmdLine);
void execute_D(cmdLine * pCmdLine);
char * fullDes(char * arg);
void changeDir(cmdLine * pCmdLine);
void changeDir_D(cmdLine * pCmdLine);
char * fullPath(cmdLine * pCmdLine);
char * strCmd(cmdLine * pCmdLine);


typedef struct process{
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                      /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	              /* next process in chain */
} process;


process * global_proc;

void addProcess(process** process_list, cmdLine* cmd, pid_t pid);
void printProcessList(process** process_list);
void printProc(process* proc);
void printCurrentProc(process * proc);


int main(int argc, char * argv[]){
    global_proc = NULL;
    if(argc == 2 && strcmp("-d" , argv[1]) == 0){
        while(1){
            char input[2048];
            fgets(input,2048,stdin);
            cmdLine * CL = parseCmdLines(input);
            printf("%p\n" , CL);
            if(CL->argCount ==1 && strcmp("quit" , CL->arguments[0]) == 0){
                freeCmdLines(CL);
                break;
            }
            execute_D(CL);
        }
    }else
    {
        while(1){
            char input[2048];
            fgets(input,2048,stdin);
            cmdLine * CL = parseCmdLines(input);
            if(CL->argCount ==1 && strcmp("quit" , CL->arguments[0]) == 0){
                freeCmdLines(CL);
                break;
            }
            execute(CL);
        }
    }
    return 0;
}




void execute(cmdLine * pCmdLine){
    if(pCmdLine->blocking == 1)
        waitpid(getpid() , NULL, 0);
    if(strcmp(pCmdLine->arguments[0] , "cd") == 0)
        changeDir(pCmdLine);
    else if(strcmp(pCmdLine->arguments[0] , "proc") == 0){
        printProcessList(&global_proc);
    }
    else{
        char * fullName = fullDes(pCmdLine->arguments[0]);
        if(fork() == 0){
            if(execvp(fullName , pCmdLine->arguments) < 0){
                perror("Fault");
                free(fullName);
                _exit(EXIT_FAILURE);
            }
        free(fullName);
        }
        addProcess(&global_proc , pCmdLine , getpid());
    }
}

void execute_D(cmdLine * pCmdLine){
    if(pCmdLine->blocking == 1)
        waitpid(getpid() , NULL, 0);
    if(strcmp(pCmdLine->arguments[0] , "cd") == 0)
        changeDir_D(pCmdLine);
    else if(strcmp(pCmdLine->arguments[0] , "proc") == 0){
        printProcessList(&global_proc);
    }
    else{
        char * fullName = fullDes(pCmdLine->arguments[0]);
        if(fork() == 0){
            if(execvp(fullName , pCmdLine->arguments) < 0){
                perror("Fault");
                free(fullName);
                _exit(EXIT_FAILURE);
            }
        }else{
            addProcess(&global_proc , pCmdLine , getpid());
            printf("PID: %d     |     Exectuting command:" , getpid());
            for(int i = 0; i < pCmdLine->argCount; i++)
                printf(" %s" , pCmdLine->arguments[i]);
            printf("\n");
        }
        free(fullName);
    }
}

void changeDir(cmdLine * pCmdLine){
    char * path = fullPath(pCmdLine);
    if(chdir(path) < 0)
        perror("Fault");
    free(path);
}

void changeDir_D(cmdLine * pCmdLine){
    char * path = fullPath(pCmdLine);
    if(chdir(path) < 0){
        perror("Fault");
    }else{
        printf("PID: %d     |     Exectuting command:" , getpid());
        for(int i = 0; i < pCmdLine->argCount; i++)
            printf(" %s" , pCmdLine->arguments[i]);
        printf("\n");
    }
    free(path);
}


char * fullDes(char * arg){
    char * buff = (char*)(malloc(PATH_MAX*sizeof(char)));
    getcwd(buff,PATH_MAX);
    strcat(buff,"/");
    strcat(buff,arg);
    return buff;
}

char * fullPath(cmdLine * pCmdLine){
    char * buff = (char*)(malloc(PATH_MAX*sizeof(char)));
    for(int i = 1; i < pCmdLine->argCount; i++){
        if(i > 1){
            strcat(buff , " ");
        }
        strcat(buff , pCmdLine->arguments[i]);
    }
    return buff;
}

char * strCmd(cmdLine * pCmdLine){
    char * buff = (char*)(calloc(PATH_MAX*sizeof(char) , sizeof(char)));
    for(int i = 0; i < pCmdLine->argCount; i++){
        if(i > 0){
            strcat(buff , " ");
        }
        strcat(buff , pCmdLine->arguments[i]);
    }
    return buff;
}
 

void addProcess(process ** process_list, cmdLine* cmd, pid_t pid){
    process * new_proc = (process *)(malloc(sizeof(process)));
    new_proc->cmd = cmd;
    new_proc->pid = pid;
    new_proc->status = RUNNING;
    if(process_list == NULL){
        new_proc->next = NULL;
    }else{
        new_proc->next = (*process_list);
    }
    global_proc = new_proc;
}


void printProcessList(process ** process_list){
    printf("PID     Command     STATUS\n");
    if(process_list == NULL)
        return;
    printCurrentProc((*process_list));
}

void printCurrentProc(process * proc){
    if(proc == NULL)
        return;
    printProc(proc);
    printCurrentProc(proc->next);
}


void printProc(process* proc){
    char * prc_command = strCmd(proc->cmd);
    switch (proc->status)
    {
    case TERMINATED:
        printf("%d      %s      TERMINATED\n" , proc->pid , prc_command);
        break;
    case RUNNING:
        printf("%d      %s      RUNNING\n" , proc->pid , prc_command);
        break;
    case SUSPENDED:
        printf("%d      %s      SUSPENDED\n" , proc->pid , prc_command);
        break;
    }
    free(prc_command);
}

