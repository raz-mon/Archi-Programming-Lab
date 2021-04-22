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
        char buf[];
    } ent;
    
int main(int argc, char* argv[], char* envp[]){

    system_call(4, 1, "flame strike yet again!\n", 25);
    
    char buff[10000];
    int fd;
    ent* entp = buff;
    int count;

    fd = system_call(5, ".", 0, 0777);
    if (fd < 0){
        system_call(4, 1, "Could not do open file directory!", 31);
        system_call(1, 0, 0,0777);
    }
        
    count = system_call(141, fd, buff, 10000);
    if (count < 0){
        system_call(4, 1, "Could not do getDents command!", 31);
        system_call(1, 0, 0,0777);
    }

    int i = 0;
    while (i < count){
        entp = buff + i;
        system_call(4, 1, "i = ", 5);
        system_call(4, 1, itoa(i), strlen(itoa(i)));
        system_call(4, 1, "\n", 1);   
        
        system_call(4, 1, itoa(entp->inode), strlen(itoa(entp->inode)));
        system_call(4, 1, "\n", 1);   
        system_call(4, 1, itoa(entp->offset), strlen(itoa(entp->offset)));
        system_call(4, 1, "\n", 1);   
        system_call(4, 1, itoa(entp->len), strlen(itoa(entp->len)));
        system_call(4, 1, "\n", 1);   
        system_call(4, 1, entp->buf, strlen(entp->buf)); 
        system_call(4, 1, "\n", 1);

        i += entp->len;
    }

        int j = system_call(6, fd, 0, 0);
        if (j < 0){
            system_call(4, 2, "Could not close the file!!", 27);
            system_call(1, 0x55, 0, 0);
        }

    return 0;
}