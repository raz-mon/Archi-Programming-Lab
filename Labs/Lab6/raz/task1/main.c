#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main_with_debug(int argc,char* argv[]);

int main(int argc, char* argv[]){
    if (strcmp(argv[argc-1], "-d") == 0){
        main_with_debug(argc, argv);
        return 1;
    }

    char* args[3];
    args[0] = "ls";
    args[1] = "-l";
    args[2] = NULL;

    char* args2[4];
    args2[0] = "tail";
    args2[1] = "-n";
    args2[2] = "2";
    args2[3] = NULL;


    int pipefd[2];
    pipe(pipefd);
    int pid = fork();
    if (pid == 0){                              // Child_1 process.
        close(STDOUT_FILENO);
        dup(pipefd[1]);
        close(pipefd[1]);
        // execute "ls -l".
        if (execvp(args[0], args) < 0){
            fprintf(stderr, "executing ls has failed.\n");
            exit(0);
        }
    }
    else{                                       // Main process.
        close(pipefd[1]);
        int pid_2 = fork();
        if (pid_2 == 0){                        // Child_2 process.
            close(STDIN_FILENO);
            dup(pipefd[0]);
            close(pipefd[0]);
            // execute "tail -n 2".
            if (execvp(args2[0], args2) < 0){
                fprintf(stderr, "executing tail has failed.\n");
                exit(0);
            }
            else{                                   // Main process.
                int a, b;
                close(pipefd[0]);
                waitpid(pid, &a, 0777);
                waitpid(pid_2, &b, 0777);        
            }
        }   
    }
}


int main_with_debug(int argc, char* argv[]){

    char* args[3];
    args[0] = "ls";
    args[1] = "-l";
    args[2] = NULL;

    char* args2[4];
    args2[0] = "tail";
    args2[1] = "-n";
    args2[2] = "2";
    args2[3] = NULL;


    int pipefd[2];
    pipe(pipefd);
    fprintf(stderr, "(parent_process>forking…)\n");
    int pid = fork();
    if (pid == 0){                                                         // Child_1 process.
        fprintf(stderr, "(child1>redirecting stdout to the write end of the pipe…)\n");
        close(STDOUT_FILENO);
        dup(pipefd[1]);
        close(pipefd[1]);
        fprintf(stderr, "(child1>going to execute cmd: …)\n");
        // execute "ls -l".
        if (execvp(args[0], args) < 0){
            fprintf(stderr, "executing ls has failed.\n");
            exit(0);
        }
    }
    else{                                                                  // Main process.
        fprintf(stderr, "(parent_process>created process with id: %d)\n", pid);
        fprintf(stderr, "(parent_process>closing the write end of the pipe…)\n");
        close(pipefd[1]);
        int pid_2 = fork();
        if (pid_2 == 0){                                                   // Child_2 process.
            fprintf(stderr, "(child2>redirecting stdin to the read end of the pipe…)\n");
            close(STDIN_FILENO);
            dup(pipefd[0]);
            close(pipefd[0]);
            fprintf(stderr, "(child2>going to execute cmd: …)\n");
            // execute "tail -n 2".
            if (execvp(args2[0], args2) < 0){
                fprintf(stderr, "executing tail has failed.\n");
                exit(0);
            }
        }
    else{                                                                 // Main process.
            int a, b;
           fprintf(stderr, "(parent_process>closing the read end of the pipe…)\n");
            close(pipefd[0]);
            fprintf(stderr, "(parent_process>waiting for child processes to terminate…)\n");
            waitpid(pid, &a, 0777);
            waitpid(pid_2, &b, 0777);        
            fprintf(stderr, "(parent_process>exiting…)\n");
        }
    }
}