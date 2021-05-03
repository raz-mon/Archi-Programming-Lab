#include <linux/limits.h>
#include "./LineParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>


int debug_mode;

void execute(cmdLine* pCmdLine){
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
    }
    else{
        int i = fork();
        if (i == 0){
            int a = execvp(real_path, pCmdLine->arguments);
            if (a < 0 ){
                freeCmdLines(pCmdLine);
                perror("Could not execute the command!!");
                exit(1);
            }
        }
        freeCmdLines(pCmdLine);   
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