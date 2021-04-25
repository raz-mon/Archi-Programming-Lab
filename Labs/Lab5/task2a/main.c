#include <linux/limits.h>
#include "./LineParser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define TERMINATED  -1
#define RUNNING 1
#define SUSPENDED 0
typedef struct process{
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	                  /* next process in chain */
} process;
int debug_mode = 0;
void execute(cmdLine* pCmdLine);
process* processList = NULL;

int main(int argc, char* argv[]){
    //processList = NULL;

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

    if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'd')
        debug_mode = 1;        // Debugger mode.
    
    execute(pCmdLine);

    }
}




