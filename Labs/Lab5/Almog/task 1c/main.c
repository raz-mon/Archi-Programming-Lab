#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "LineParser.h"
#include <string.h>
#include <wait.h>

void execute(cmdLine * pCmdLine);
void execute_D(cmdLine * pCmdLine);
char * fullDes(char * arg);
void changeDir(cmdLine * pCmdLine);
void changeDir_D(cmdLine * pCmdLine);
char * fullPath(cmdLine * pCmdLine);

int main(int argc, char * argv[]){
    if(argc == 2 && strcmp("-d" , argv[1]) == 0){
        while(1){
            char input[2048];
            fgets(input,2048,stdin);
            cmdLine * CL = parseCmdLines(input);
            if(CL->argCount ==1 && strcmp("quit" , CL->arguments[0]) == 0){
                freeCmdLines(CL);
                break;
            }
            execute_D(CL);
            freeCmdLines(CL);
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
            freeCmdLines(CL);
        }
    }
    return 0;
}




void execute(cmdLine * pCmdLine){
    if(pCmdLine->blocking == 1)
        waitpid(getpid() , NULL, 0);
    if(strcmp(pCmdLine->arguments[0] , "cd") == 0)
        changeDir(pCmdLine);
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
    }
}

void execute_D(cmdLine * pCmdLine){
    if(pCmdLine->blocking == 1)
        waitpid(getpid() , NULL, 0);
    if(strcmp(pCmdLine->arguments[0] , "cd") == 0)
        changeDir_D(pCmdLine);
    else{
        char * fullName = fullDes(pCmdLine->arguments[0]);
        if(fork() == 0){
            if(execvp(fullName , pCmdLine->arguments) < 0){
                perror("Fault");
                free(fullName);
                _exit(EXIT_FAILURE);
            }
        }else{
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
 
