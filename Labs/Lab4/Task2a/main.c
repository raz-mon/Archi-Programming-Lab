#include "./lab4_util.h"

int system_call();
void encoder();
void encoder_in(char* filename);
void encoder_out(char* filename);
void encoder_in_and_out(char* in_name, char* out_name);

typedef struct ent {
        int inode;
        int offset;
        short len;
        char buf[1];
    } ent;
    
int main(int argc, char* argv[], char* envp[]){
    
    char buff[10000];
    int fd;
    ent* entp = buff;
    int count;

    fd = system_call(5, ".", 0, 0);
    if (fd < 0)
        system_call(1, 0, 0,0);
    
    count = system_call(141, fd, buff, 10000);



    system_call(4, 1, entp->buf, strlen(entp->buf));
    entp = entp + entp->len;
    system_call(4, 1, entp->buf, strlen(entp->buf));
    entp = entp + entp->len;
    system_call(4, 1, entp->buf, strlen(entp->buf));
    
    return 0;
}