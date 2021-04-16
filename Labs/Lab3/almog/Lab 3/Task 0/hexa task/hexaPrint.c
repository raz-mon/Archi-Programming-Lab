#include <stdio.h>
#include <stdlib.h>

void PrintHex(FILE * buffer, int length);

int main(int argc, char **argv){
    FILE* f = fopen(argv[1], "r");
    fseek(f,0,SEEK_END);
    int len = ftell(f);
    fseek(f,0,SEEK_SET);
    PrintHex(f,len);
    return 0;
}

void PrintHex(FILE * buffer, int length){
    unsigned char ch[length];
    fread(&ch, 1, length,buffer);
    for(int i = 0; i < length; ++i){
        printf("%02X " , ch[i]);
    }

    printf("\n");
    fclose(buffer);
}