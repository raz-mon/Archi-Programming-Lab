#include "lab4_util.h"

 #define BUF_SIZE 1024

#define SYS_EXIT 1
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEEK 19
#define SYS_GETDENTS 141


#define STDIN 0
#define STDOUT 1

#define O_RDONLY 0
#define O_WRONLY 1

typedef struct ent{
  int inode;
  int offset;
  short len;
  char buf[];
}ent;

int system_call();

int main (int argc , char* argv[], char* envp[]){
  int fd, nread, curr;
  char buf[BUF_SIZE];
  struct ent *d;

  fd = system_call(SYS_OPEN,".",O_RDONLY,0777);
  if(fd < 0)
    system_call(SYS_EXIT,0x55,0,0);

  nread = system_call(SYS_GETDENTS, fd, buf, BUF_SIZE);
  if(nread < 0)
    system_call(SYS_EXIT,0x55,0,0);
  else if(nread == 0)
    system_call(SYS_EXIT,0,0,0);

  for (curr = 0; curr < nread;) {
    d = (struct ent *) (buf + curr);
    system_call(SYS_WRITE,STDOUT,d->buf,strlen(d->buf));
    system_call(SYS_WRITE,STDOUT,"\n",1);
    curr += d->len;
  }
  return 0;
}