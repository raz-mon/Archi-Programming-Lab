#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main_with_debug(int argc,char* argv[]);

int main(int argc, char* argv[]){
    if (strcmp(argv[argc-1], "-d"))
        main_with_debug(argc, argv);
    int pipefd[2];
    int pid = fork();
    if (pid == 0){                              // Child_1 process.
        close(STDOUT_FILENO);
        dup(pipefd[1]);
        close(pipefd[1]);
        // execute "ls -l".
    }
    else{                                       // Main process.
        close(pipefd[1]);
        int pid_2 = fork();
        if (pid_2 == 0){                        // Child_2 process.
            close(STDIN_FILENO);
            dup(pipefd[0]);
            close(pipefd[0]);
            // execute "tail -n 2".
        }
        else{                                   // Main process.
            int a, b;
            close(pipefd[0]);
            waitpid(pid, &a, 0777);
            waitpid(pid_2, &b, 0777);        
        }
    }
}


int main_with_debug(int argc, char* argv[]){
    int pipefd[2];
    fprintf(stderr, "(parent_process>forking…)");
    int pid = fork();
    fprintf(stderr, "(parent_process>created process with id: %d)", pid);
    if (pid == 0){                                                         // Child_1 process.
        fprintf(stderr, "(child1>redirecting stdout to the write end of the pipe…)");
        close(STDOUT_FILENO);
        dup(pipefd[1]);
        close(pipefd[1]);
        fprintf(stderr, "(child1>going to execute cmd: …)");
        // execute "ls -l".
    }
    else{                                                                  // Main process.
        fprintf(stderr, "(parent_process>closing the write end of the pipe…)");
        close(pipefd[1]);
        int pid_2 = fork();
        if (pid_2 == 0){                                                   // Child_2 process.
            fprintf(stderr, "(child2>redirecting stdin to the read end of the pipe…)");
            close(STDIN_FILENO);
            dup(pipefd[0]);
            close(pipefd[0]);
            fprintf(stderr, "(child2>going to execute cmd: …)");
            // execute "tail -n 2".
        }
    else{                                                                 // Main process.
            int a, b;
           fprintf(stderr, "(parent_process>closing the read end of the pipe…)");
            close(pipefd[0]);
            fprintf(stderr, "(parent_process>waiting for child processes to terminate…)");
            waitpid(pid, &a, 0777);
            waitpid(pid_2, &b, 0777);        
            fprintf(stderr, "(parent_process>exiting…)");
        }
    }
}