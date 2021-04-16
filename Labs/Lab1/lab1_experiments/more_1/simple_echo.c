

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void getAns(char* ans);

int main(int argc, char* argv[]){
 //   printf("Whats up? type somthing:");
 //   char ans[256];
 //   getAns(ans);
    FILE * output = stdout;
    
    for (int i = 1; i < argc; ++i){
        printf(argv[i]);
        printf(" ");
    }
    printf("\n");


    /*
    for (int i = 1; i < argc; ++i){
        fprintf(output, argv[i]);
        fprintf(output, " ");
    }
    fprintf(output, "\n");
    */
    return 1;
}