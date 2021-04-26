#include "./lab4_util.h"

int system_call();

typedef struct ent {
        int inode;
        int offset;
        short len;
        char buf[];
    } ent;

void print_names(char* buff, int fd, ent* entp, int count){
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
}

void print_dtype(char dtype){
    char* str = dtype == 0 ? "DT_UNKNOWN" : 
    dtype == 1 ? "FIFO" :
    dtype == 2 ? "DT_CHR" :
    dtype == 4 ? "DIR" :
    dtype == 6 ? "DT_BLK" :
    dtype == 8 ? "DT_REG" :
    dtype == 10 ? "DT_LNK" :
    dtype == 12 ? "DT_SOCK" :
    dtype == 14 ? "DT_WHT" :
    "???";
    system_call(4, 1, str, strlen(str));
}

void print_by_prefix(char prefix, char* buff, int fd, ent* entp, int count){

    system_call(4, 1, "prefix is: ", 12);
    system_call(4, 1, &prefix, 1);
    system_call(4, 1, "\n", 1);   

    int i = 0;
    while (i < count){

        entp = buff + i;
        
        if (entp->buf[0] == prefix){
            system_call(4, 1, "b_pos = ", 9);
            system_call(4, 1, itoa(i), strlen(itoa(i)));
            system_call(4, 1, "\n", 1);   

            system_call(4, 1, "inode: ", 8);  
            system_call(4, 1, itoa(entp->inode), strlen(itoa(entp->inode)));
            system_call(4, 1, "\n", 1);   

            system_call(4, 1, "offset: ", 9);
            system_call(4, 1, itoa(entp->offset), strlen(itoa(entp->offset)));
            system_call(4, 1, "\n", 1);  

            system_call(4, 1, "len: ", 6);
            system_call(4, 1, itoa(entp->len), strlen(itoa(entp->len)));
            system_call(4, 1, "\n", 1);   
            
            system_call(4, 1, "buf: ", 6);
            system_call(4, 1, entp->buf, strlen(entp->buf)); 
            system_call(4, 1, "\n", 1);

            system_call(4, 1, "dtype: ", 8);
            char dtype = *(buff + i + entp->len - 1);
            print_dtype(dtype);
            system_call(4, 1, "\n", 1);
        }
        i += entp->len;
    }
        int j = system_call(6, fd, 0, 0);
        if (j < 0){
            system_call(4, 2, "Could not close the file!!", 27);
            system_call(1, 0x55, 0, 0);
        }
    
}




