
#include <stdio.h>
#include <stdlib.h>

void getAns(char* ans){
    
    int i = 0;
    int c;

    while ((c = fgetc(stdin)) != '\n'){
        ans[i++] = (char) c;
    }
    
}