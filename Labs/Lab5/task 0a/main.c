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
    
    char * input;
    fgets(input,2048,stdin);
    //cmdLine * CL = parseCmdLines(input);
    //printf("Number of arguments: %d\n" , CL->argCount);

    printf("Arguments: ");
    //for(int i =0; i < CL->argCount ; i++){
    //    printf(" %s" , CL->arguments[i]);
    //}
    printf("\n");
    //execute(CL);
    
    char * test2[] = {"./Test111" , NULL };
    //execv("hello" , CL->arguments);
    //perror("Fault");
    return 0;
}



/*
void execute(cmdLine * pCmdLine){
    FILE * f = fopen("Test111" , "w+");
    //fclose(f);
    char * test2[] = {"./Test111" , (char *)0 };
    //int test = 
    execv("./Test111", test2);
    perror("Fault");
    //printf("%d" , test);
}
*/