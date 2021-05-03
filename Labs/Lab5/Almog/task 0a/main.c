#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "LineParser.h"
#include <string.h>

void execute(cmdLine * pCmdLine);
char * fullDes(char * arg);

int main(int argc, char * argv[]){
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
    return 0;
}




void execute(cmdLine * pCmdLine){
    char * fullName = fullDes(pCmdLine->arguments[0]);
    if(fork() == 0){
        if(execvp(fullName , pCmdLine->arguments) < 0){
            perror("Fault");
            free(fullName);
            exit(EXIT_FAILURE);
        }
    }
    free(fullName);
}

char * fullDes(char * arg){
    char * buff = (char*)(malloc(PATH_MAX*sizeof(char)));
    getcwd(buff,PATH_MAX);
    strcat(buff,"/");
    strcat(buff,arg);
    return buff;
} 
