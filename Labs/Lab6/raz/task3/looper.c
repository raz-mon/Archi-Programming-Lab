#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>


void handler(int signum){
	//char* signum_str = strsignal(signum);
	//printf("recieved signal : %s ", strsignal(signum));
	printf("\nrecieved signal: %s\n", strsignal(signum));
	signal(signum, SIG_DFL);
	raise(signum);
	switch(signum)
	{
		case(SIGSTOP):
			signal(SIGCONT, handler);
			break;
		case(SIGCONT):
			signal(SIGTSTP, handler);
			break;
	}
}

int main(int argc, char **argv){ 
	printf("Starting the program\n");
	signal(SIGINT, handler);
	signal(SIGTSTP, handler);
	signal(SIGCONT, handler);
	while(1) {
		sleep(2);
	}
	return 0;
}