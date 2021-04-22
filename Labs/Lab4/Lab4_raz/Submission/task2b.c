#include "./lab4_util.h"

typedef struct ent {
        int inode;
        int offset;
        short len;
        char buf[];
    } ent;

int system_call();
void encoder();
void encoder_in(char* filename);
void encoder_out(char* filename);
void encoder_in_and_out(char* in_name, char* out_name);
void print_by_prefix(char prefix, char* buff, int fd, ent* entp, int count);
void print_names(char* buff, int fd, ent* entp, int count);
    
int main(int argc, char* argv[], char* envp[]){

    char buff[10000];
    int fd;
    ent* entp = (ent*) buff;
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

    if (argc > 1){
        switch(argv[1][1])
        {
            case 'p':
                print_by_prefix(argv[1][2], buff, fd, entp, count);
                break;
            case 'a':
                system_call(4, 1, "TBD\n", 4);
        }
    }
    else
        print_names(buff, fd, entp, count);


    return 0;
}