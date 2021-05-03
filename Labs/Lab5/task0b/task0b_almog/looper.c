#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void sigprint_hendler(int sig);

int main(int argc, char **argv){ 
	signal(SIGTSTP , sigprint_hendler);
	signal(SIGINT , sigprint_hendler);
	signal(SIGCONT , sigprint_hendler);
	
	printf("Starting the program\n");

	while(1) {
		sleep(2);
	}

	return 0;
}

void sigprint_hendler(int sig){
	printf("\n%s has received\n" , strsignal(sig));
	signal(sig , SIG_DFL);
	raise(sig);
	switch (sig)
	{
		case SIGCONT:
			signal(SIGTSTP , sigprint_hendler);
			break;
		case SIGTSTP:
			signal(SIGCONT , sigprint_hendler);
			break;
	}
}