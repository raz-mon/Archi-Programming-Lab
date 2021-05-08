#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <wait.h>

int main(int argc, char * argv[]){
    char * args1[3];
    args1[0] = "ls";
    args1[1] = "-l";
    args1[2] = NULL;

    char * args2[4];
    args2[0] = "tail";
    args2[1] = "-n";
    args2[2] = "2";
    args1[3] = NULL;

    int pipefd[2];
    pipe(pipefd);
    pid_t pid1 = fork();
    if( pid1 == 0){
        close(STDOUT_FILENO);
        int newfiled = dup(pipefd[1]);
        if(newfiled == -1){
            perror("dup");
            _exit(EXIT_FAILURE);
        }
        close(pipefd[1]);
        execvp(args1[0], args1);
    }

    close(pipefd[1]);
    pid_t pid2 = fork();
    if( pid2 == 0){
        close(STDIN_FILENO);
        int newfiled = dup(pipefd[0]);
        if(newfiled == -1){
            perror("dup");
            _exit(EXIT_FAILURE);
        }
        close(pipefd[1]);
        pipefd[1] = newfiled;
        execvp(args2[0], args1);
    }
    close(pipefd[0]);
    waitpid(pid1, NULL,0);
    waitpid(pid2, NULL,0);
    return 0;
}