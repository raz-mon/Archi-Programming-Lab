#include <linux/limits.h>
#include "./LineParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void execute(cmdLine* pCmdLine){
    char path[PATH_MAX];
    getcwd(path, PATH_MAX);
    char* real_path = strcat(path, "/");
    strcat(path, pCmdLine->arguments[0]);
    if (fork() == 0){
        int a = execvp(real_path, pCmdLine->arguments);
        if (a < 0 ){
            freeCmdLines(pCmdLine);
            perror("Could not execute the command!!");
            exit(1);
        }
    }
    freeCmdLines(pCmdLine);
}

int main(int argc, char* argv[]){
    
    // Print the current directory
    char path[PATH_MAX];
    getcwd(path, PATH_MAX);
    printf("current directory: %s\n", path);
    

    while(1){

    //Read a line from from the user
    char line[2048];
    fgets(line, 2048, stdin);

    //parseCmdLines
    cmdLine* pCmdLine = parseCmdLines(line);
    if (strcmp(pCmdLine->arguments[0], "quit") == 0)
        exit(0);
    execute(pCmdLine);

    }
}




