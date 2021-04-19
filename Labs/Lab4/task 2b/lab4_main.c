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
  char d_type;

  if (argc == 2)
  {
    char c = argv[1][2];
    for (curr = 0; curr < nread;) {
      d = (struct ent *) (buf + curr);
      d_type = *(buf + curr + d->len -1);
      if(strncmp(&c,d->buf,1) == 0){
        system_call(SYS_WRITE,STDOUT,d->buf,strlen(d->buf));
        system_call(SYS_WRITE,STDOUT,":  ",2);
        system_call(SYS_WRITE,STDOUT,itoa(d_type),10);
        system_call(SYS_WRITE,STDOUT,"\n",1);
      }
      curr += d->len;
    }
  }else{
    for (curr = 0; curr < nread;) {
      d = (struct ent *) (buf + curr);
      d_type = *(buf + curr + d->len -1);
      system_call(SYS_WRITE,STDOUT,d->buf,strlen(d->buf));
      system_call(SYS_WRITE,STDOUT,":  ",2);
      system_call(SYS_WRITE,STDOUT,itoa(d_type),10);
      system_call(SYS_WRITE,STDOUT,"\n",1);
      curr += d->len;
    }
  }
  return 0;
}

/*
void printDtype(char d_type){
  (d_type == DT_REG) ?  system_call(SYS_WRITE,STDOUT,"regular\n",9) :
  (d_type == DT_DIR) ?  system_call(SYS_WRITE,STDOUT,"directory\n",11) :
  (d_type == DT_FIFO) ? system_call(SYS_WRITE,STDOUT,"FIFO\n",6) :
  (d_type == DT_SOCK) ? system_call(SYS_WRITE,STDOUT,"socket\n",8) :
  (d_type == DT_LNK) ?  system_call(SYS_WRITE,STDOUT,"symlink\n",9) :
  (d_type == DT_BLK) ?  system_call(SYS_WRITE,STDOUT,"block dev\n",11) :
  (d_type == DT_CHR) ?  system_call(SYS_WRITE,STDOUT,"char dev\n",10) :
   system_call(SYS_WRITE,STDOUT,"???\n",4);
}
*/