#include <linux/limits.h>
#include "./LineParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    /*
    // Print the current directory
    char path[PATH_MAX];
    getcwd(path, PATH_MAX);
    printf("%s", path);
    printf("\n");
    */

    //Read a line from from the user
    char line[2048];
    fgets(line, 2048, stdin);

    //parseCmdLines
    cmdLine* pCmdLine = parseCmdLines(line);
    printf("arguments: %s\n", pCmdLine->);
    //execute(pCmdLine);
}


void execute(cmdLine* pCmdLine){
    char path[PATH_MAX];
    getcwd(path, PATH_MAX);
    int a = execv(path, pCmdLine->arguments);
    printf("%d\n", a);
}

