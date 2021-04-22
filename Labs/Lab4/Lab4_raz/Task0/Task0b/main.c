#include "./lab4_util.h"

#define SYS_WRITE 4
#define STDOUT 1

int system_call();

int main (int argc , char* argv[], char* envp[])
{
  int fd = system_call(5, "greeting", 2, 0777);
  if (fd < 0){
    system_call(1, 0x55, 0, 0);
  }
  system_call(19, fd, 0x291, 0);
  system_call(4, fd, "Mira. ", 6);

  return 0;
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
      system_call(SYS_WRITE,STDOUT, "Hello world!\n", 13);
    }
  */
}
