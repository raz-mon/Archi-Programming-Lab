#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "LineParser.h"

void execute(cmdLine * pCmdLine);

int main(int argc, char ** argv){

    char * input;
    fgets(input,2048,stdin);
    cmdLine * CL = parseCmdLines(input);
    execute(CL);

    return 0;
}

void execute(cmdLine * pCmdLine){
    char buff[PATH_MAX];
    getcwd(buff,PATH_MAX);
    printf("%s\n" , buff);

    int test = execv(buff, pCmdLine->arguments);
    printf("%d" , test);
}