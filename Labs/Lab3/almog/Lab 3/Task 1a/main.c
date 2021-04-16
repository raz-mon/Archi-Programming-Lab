#include <stdio.h>
#include <stdlib.h>

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

void PrintHex(FILE * buffer, int length);
virus* readVirus(FILE * file);
void printVirus(virus* virus, FILE * output);

int main(int argc, char **argv){
    FILE * file = fopen(argv[1] , "r");

    fseek(file,0,SEEK_END);
    int file_size = ftell(file);

    fseek(file,4,SEEK_SET);
    while (ftell(file) < file_size)
    {
        virus * v = readVirus(file);
        printVirus(v,stdout);
        free(v->sig);
        free(v);
    }
    fclose(file);
    return 0;
}


virus* readVirus(FILE * file){
    virus * v = malloc(sizeof(virus));
    fread(v, sizeof(char) , 18 , file);
    (*v).sig = (unsigned char *)malloc(sizeof(char) * (*v).SigSize);
    fread(v->sig, sizeof(char) , (*v).SigSize , file);
    return v;
}

void PrintHex(FILE * buffer, int length){
    unsigned char ch[length];
    fread(&ch, 1, length,buffer);
    for(int i = 0; i < length; ++i){
        printf("%02X " , ch[i]);
    }
    printf("\n");
}

void printVirus(virus* virus, FILE * output){
    fprintf(output, "Virus size: %d\nVirus name: %s\nVirus signature:\n", virus->SigSize , virus->virusName);
    for(int i = 0; i < virus->SigSize; ++i){
        fprintf(output, "%02X " , virus->sig[i]);
    }
    fprintf(output,"\n\n");
}