

#include <stdlib.h>
#include <stdio.h>


int printHex(char* buffer, int length);
void getFileName(char* FN, char** argv);

int main(int argc, char* argv[]){

    //char* FN = argv[1];

    unsigned char* buffer = (char *)malloc(30 * sizeof(char));
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    int i = fread(buffer, sizeof(char), 30, file);
    // size_t ret = fread(buffer, ARRAY_SIZE(buffer), sizeof(*buffer), fp);
    
    printf("byte have been read: %d\n", i);

    printHex(buffer, i);
    // Should print: 63 68 65 63 6B AA DD 4D 79 0C 48 65 78

    printf("%s", "\n");

    fclose(file);
    free(buffer);

    return 1;
}