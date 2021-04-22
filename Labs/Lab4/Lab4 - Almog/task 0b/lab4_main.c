#include "lab4_util.h"

#define SYS_EXIT 1
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_LSEEK 19
#define STDOUT 1

int system_call();

int main (int argc , char* argv[], char* envp[])
{
  /*
  int i;
  char * str;
  str = itoa(argc);
  system_call(SYS_WRITE,STDOUT, str,strlen(str));
  system_call(SYS_WRITE,STDOUT," arguments \n", 12);
  for (i = 0 ; i < argc ; i++)
    {
      system_call(SYS_WRITE,STDOUT,"argv[", 5);
	  str = itoa(i);
      system_call(SYS_WRITE,STDOUT,str,strlen(str));
      system_call(SYS_WRITE,STDOUT,"] = ",4);
      system_call(SYS_WRITE,STDOUT,argv[i],strlen(argv[i]));
      system_call(SYS_WRITE,STDOUT,"\n",1);
    }
    system_call(SYS_WRITE,STDOUT,"Hello World\n",12);
    */

    int file = system_call(SYS_OPEN,"greeting",2,0777);
    if(file < 0){
      system_call(SYS_EXIT,0x55,0,0);
    }
    int lseek = system_call(SYS_LSEEK,file,0x291,0);
    if(lseek < 0){
      system_call(SYS_EXIT,0x55,0,0);
    }
    int fwrite = system_call(SYS_WRITE,file,"Mira. ",6);
    if(fwrite < 0){
      system_call(SYS_EXIT,0x55,0,0);
    }

  return 0;
}
