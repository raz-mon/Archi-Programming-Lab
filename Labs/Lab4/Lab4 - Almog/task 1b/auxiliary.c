#include "lab4_util.h"

#define SYS_EXIT 1
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEEK 19

#define STDIN 0
#define STDOUT 1

int system_call();
void LowToCap(int path, int buffer);


void LowToCap(int path, int buffer){
    while (1){
        char c[1];
        int read = system_call(SYS_READ,buffer,c,1);
        if (*c >= 65 && *c <= 90){
            *c += 32;
        }else if(read == 0){
            if(buffer != STDIN)
                system_call(SYS_WRITE,path,"\n",1);
            system_call(SYS_EXIT,0,0,0);
        }
        system_call(SYS_WRITE,path,c,1);
    }
}