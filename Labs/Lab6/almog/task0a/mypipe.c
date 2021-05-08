#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

int main(int argc, char * argv[]){

    int pipefd[2];
    pid_t pid;

    pipe(pipefd);

    pid = fork();
    if( pid == 0 ){
        write(pipefd[1], "hello\n", 7);
        close(pipefd[1]);
        _exit(EXIT_SUCCESS);
    }else{
        char buff[7];
        read(pipefd[0], buff, 7);
        printf("%s" , buff);
        wait(NULL);
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    }



    return 0;
}