

#include <stdlib.h>
#include <stdio.h>



typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

int printHex(unsigned char* buffer, int length){
    int printed = 0;
    for (int i = 0; i < length; ++i){
        printf("%02X ", buffer[i]);
        printed++;
    }
    return printed;
}

virus* readVirus(FILE* file){
    virus* Virus = (virus *)malloc(sizeof(virus));
    //fread(Virus, sizeof(short), 1, file);
    //fread(Virus->virusName, sizeof(char), 16, file);
    fread(Virus, sizeof(char), 18, file);
    Virus->sig = (unsigned char *) malloc(sizeof(char) * Virus->SigSize);
    fread(Virus->sig, sizeof(char), Virus->SigSize, file);
    return Virus;
}

void printVirus(virus* virus, FILE* output){
    printf("Virus name: %s\n", virus->virusName);
    printf("Virus size: %d\nsignature:\n", virus->SigSize);
    printHex(virus->sig, virus->SigSize);
    printf("\n");
}




