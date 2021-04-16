

#include <stdlib.h>
#include <stdio.h>

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

int printHex(unsigned char* buffer, int length);
virus* readVirus(FILE* file);
void printVirus(virus* virus, FILE* output);

int main(int argc, char* argv[]){
    
    FILE* file = fopen(argv[1], "r");



    //  read all viruss' from signatues-L file, and print.
    fseek(file, 0, SEEK_END);
    int end = ftell(file);
    fseek(file, 4, SEEK_SET);       // put the offset after 4 first bytes (irrelevant right now).
    do{
        virus* v = readVirus(file);
        printVirus(v, stdout);
        free(v->sig);
        free(v);
    } while (ftell(file) != end);

    return 1;
}























    /*virus* first_v = readVirus(file);
    printf("%d, %s, %02X\n", first_v->SigSize, first_v->virusName, first_v->sig);
    */


     /*
    // Find size of the file:
    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);        // file_size holds the size of the whole file in bytes.
    fseek(file, 0, SEEK_SET);

    char* buffer = (char *)malloc(sizeof(char) * file_size);        // buffer with the size of the file.

    int i = fread(buffer, sizeof(char), file_size, file);       // put the content of the file in the buffer (character array).
    // size_t ret = fread(buffer, ARRAY_SIZE(buffer), sizeof(*buffer), fp);      Signature of fread().
    
    //printf("%d bytes have been read.\n", i);      // The amount of bytes read by fread().

    //printHex(buffer, i);                          // Print the content of the buffer.
    */
