
#include <stdlib.h>
#include <stdio.h>

int printHex(unsigned char* buffer, int length){
    int printed = 0;
    for (int i = 0; i < length; ++i){
        printf("%02X ", buffer[i]);
        printed++;
    }
    return printed;
}

void getFileName(char* FN, char** argv){
    FN = argv[1];
}