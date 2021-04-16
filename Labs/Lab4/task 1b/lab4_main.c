#include "lab4_util.h"

#define SYS_EXIT 1
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEEK 19

#define STDIN 0
#define STDOUT 1

#define O_RDONLY 0
#define O_WRONLY 1

int system_call();
void LowToCap(int path, int buffer);

int main (int argc , char* argv[], char* envp[])
{
  int file;
  if (argc == 1)
  {
      LowToCap(STDOUT,STDIN);
  }
  else if (argc == 2)
  {
    if( argv[1][1] == 'i'){
      file = system_call(SYS_OPEN,argv[1] + 2,O_RDONLY,0777);
      LowToCap(STDOUT,file);
    }
    else
    {
      file = system_call(SYS_OPEN,argv[1] + 2,O_WRONLY,0777);
      LowToCap(file,STDIN);
    }
    system_call(SYS_CLOSE,file);
  }
  else
  {
    int file2;
    if( argv[1][1] == 'i'){
      file = system_call(SYS_OPEN,argv[1] + 2,O_RDONLY,0777);
      file2 = system_call(SYS_OPEN,argv[2] + 2,O_WRONLY,0777);
      LowToCap(file2,file);
    }
    else
    {
      file = system_call(SYS_OPEN,argv[1] + 2,O_WRONLY,0777);
      file2 = system_call(SYS_OPEN,argv[2] + 2,O_RDONLY,0777);
      LowToCap(file,file2);
    }
    system_call(SYS_CLOSE,file);
    system_call(SYS_CLOSE,file2);
  }

  return 0;
}
