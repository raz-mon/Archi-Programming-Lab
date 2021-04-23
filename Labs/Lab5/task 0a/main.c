#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "LineParser.h"

void execute(cmdLine * pCmdLine);

int main(int argc, char * argv[]){
    /*
    char buff[PATH_MAX];
    getcwd(buff,PATH_MAX);
    printf("%s\n" , buff);
    */
    
    char input[2048];
    fgets(input,2048,stdin);
    cmdLine * CL = parseCmdLines(input);
    execute(CL);

    return 0;
}




void execute(cmdLine * pCmdLine){
    execv(pCmdLine->arguments[0] , pCmdLine->arguments);
    perror("Fault");
}
