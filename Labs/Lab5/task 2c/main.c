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

void freeProcessList(process* process_list);        //free all memory allocated for the process list.
void updateProcessList(process **process_list);
void updateProcessStatus(process* process_list, int pid, int status);
void deleteTerminated(process *process_list);

void suspend(cmdLine * pCmdLine);
void kill_proc(cmdLine * pCmdLine);
pid_t findPid(process *process_list , pid_t pid);


int main(int argc, char * argv[]){
    global_proc = NULL;
    if(argc == 2 && strcmp("-d" , argv[1]) == 0){
        while(1){
            char input[2048];
            fgets(input,2048,stdin);
            cmdLine * CL = parseCmdLines(input);
            if(CL->argCount ==1 && strcmp("quit" , CL->arguments[0]) == 0){
                freeCmdLines(CL);
                freeProcessList(global_proc);
                break;
            }
            execute_D(CL);
        }
    }else
    {
        while(1){



            //printf("CURRENT PID: %d\n" , getpid());



            char input[2048];
            fgets(input,2048,stdin);
            cmdLine * CL = parseCmdLines(input);
            if(CL->argCount ==1 && strcmp("quit" , CL->arguments[0]) == 0){
                freeCmdLines(CL);
                freeProcessList(global_proc);
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
    if(strcmp(pCmdLine->arguments[0] , "cd") == 0){
        changeDir(pCmdLine);
        freeCmdLines(pCmdLine);
    }
    else if(strcmp(pCmdLine->arguments[0] , "proc") == 0){
        printProcessList(&global_proc);
        freeCmdLines(pCmdLine);
    }
    else if(strcmp(pCmdLine->arguments[0] , "suspend") == 0){
        suspend(pCmdLine);
        freeCmdLines(pCmdLine);
    }
    else if(strcmp(pCmdLine->arguments[0] , "kill") == 0){
        kill_proc(pCmdLine);
        freeCmdLines(pCmdLine);
    }
    else{
        //char * fullName = fullDes(pCmdLine->arguments[0]);
        int i = fork();
        if(i == 0){
            if(execvp(pCmdLine->arguments[0] , pCmdLine->arguments) < 0){
                perror("Fault");
                //free(fullName);
                freeCmdLines(pCmdLine);
                freeProcessList(global_proc);
                exit(EXIT_FAILURE);
            }
        //free(fullName);
        }
        addProcess(&global_proc , pCmdLine , i);
    }
}

void execute_D(cmdLine * pCmdLine){
    if(pCmdLine->blocking == 1)
        waitpid(getpid() , NULL, 0);
    if(strcmp(pCmdLine->arguments[0] , "cd") == 0){
        changeDir_D(pCmdLine);
        freeCmdLines(pCmdLine);
    }
    else if(strcmp(pCmdLine->arguments[0] , "proc") == 0){
        printProcessList(&global_proc);
        freeCmdLines(pCmdLine);
    }
    else if(strcmp(pCmdLine->arguments[0] , "free") == 0){
        freeProcessList(global_proc);
        freeCmdLines(pCmdLine);
    }
    else if(strcmp(pCmdLine->arguments[0] , "suspend") == 0){
        suspend(pCmdLine);
        freeCmdLines(pCmdLine);
    }
    else if(strcmp(pCmdLine->arguments[0] , "kill") == 0){
        kill_proc(pCmdLine);
        freeCmdLines(pCmdLine);
    }
    else{
        //char * fullName = fullDes(pCmdLine->arguments[0]);
        int i = fork();
        if(i == 0){
            if(execvp(pCmdLine->arguments[0] , pCmdLine->arguments) < 0){
                perror("Fault");
                //free(fullName);
                freeCmdLines(pCmdLine);
                freeProcessList(global_proc);
                exit(EXIT_FAILURE);
            }
        }else{
            addProcess(&global_proc , pCmdLine , i);
            printf("PID: %d     |     Exectuting command:" , i);
            for(int j = 0; j < pCmdLine->argCount; j++)
                printf(" %s" , pCmdLine->arguments[j]);
            printf("\n");
        }
        //free(fullName);
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
    updateProcessList( & global_proc);
    printf("PID     Command     STATUS\n");
    if(process_list == NULL)
        return;
    printCurrentProc((*process_list));
    deleteTerminated(global_proc);
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


void freeProcessList(process* process_list){
    if(process_list != NULL){
        freeCmdLines(process_list->cmd);
        freeProcessList(process_list->next);
        free(process_list);
    }else{
        global_proc = NULL;
    }
}

void updateProcessList(process **process_list){
    if((*process_list) != NULL){
        int status;
        pid_t pid = waitpid((*process_list)->pid , &status , WNOHANG | WUNTRACED | WCONTINUED);
        printf("STATUS: %d\n" , pid);
        
        if(pid == -1){
            (*process_list)->status = TERMINATED;
        }
        else if(WIFSTOPPED(status)){
            (*process_list)->status = SUSPENDED;
        }
        else if(WIFCONTINUED(status)){
            (*process_list)->status = RUNNING;
        }
        updateProcessList(&(*process_list)->next);
    }
}

void updateProcessStatus(process* process_list, int pid, int status){
    if(process_list == NULL){ return; }
    else if(process_list->pid == pid){ process_list->status = status; return; }
    else{ updateProcessStatus(process_list->next , pid , status); }
}

void deleteTerminated(process *process_list){
    process * father = process_list;
    if(father == NULL){ return; }
    while (father->status == TERMINATED)
    {
        freeCmdLines(father->cmd);
        process * temp = father;
        father = father->next;
        free(temp);
        if(father == NULL){ global_proc = NULL; return; }
    }
    global_proc = father;
    process * son = father->next;
    while (son != NULL)
    {
        if(son->status == TERMINATED){
            freeCmdLines(son->cmd);
            process * temp = son;
            son = son->next;
            free(temp);
            father->next = son;
        }else{
            father = son;
            son = son->next;
        }
    }
}

void suspend(cmdLine * pCmdLine){
    int i = fork();
    pid_t pid = atoi(pCmdLine->arguments[1]);
    if(i == 0){
        if(pCmdLine->argCount != 3 || findPid(global_proc , pid)  == -1){
            perror("Fault: wrong arguments");
            exit(EXIT_FAILURE);
        }else{
            int cond = kill(pid, SIGTSTP);
            if(cond != 0){ perror("Cannot complete command"); exit(EXIT_FAILURE); return; }
            sleep(atoi(pCmdLine->arguments[2]));
            kill(pid, SIGCONT);
            if(cond != 0){ perror("Cannot complete command"); exit(EXIT_FAILURE); return; }
            freeProcessList(global_proc);
            exit(EXIT_SUCCESS);
        }
    }
}

void kill_proc(cmdLine * pCmdLine){
    pid_t pid = atoi(pCmdLine->arguments[1]);
    if(pCmdLine->argCount != 2 || findPid(global_proc , pid)  == -1){
        perror("Fault: wrong arguments");
        return;
    }else
    {
        int cond = kill(pid, SIGINT);
        if(cond != 0){ perror("Cannot complete command"); return; }
        //updateProcessStatus(global_proc, pid, TERMINATED);
    }
}

pid_t findPid(process *process_list , pid_t pid){
    if(process_list == NULL)
        return -1;
    if(process_list->pid == pid)
        return process_list->pid;
    findPid(process_list->next , pid);
}