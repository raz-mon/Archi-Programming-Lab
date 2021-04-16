
#include <stdio.h>

int main(int argc, char* argv[]){
    int iarray[] = {1,2,3};
    char carray[] = {'a','b','c'};
    int* iarrayPtr;
    char* carrayPtr;

    iarrayPtr = iarray;

    for(int i = 0; i < 3; ++i){
        printf("%d\n", *(iarrayPtr + i));
    }
    
    carrayPtr = carray;
    for(int i = 0; i < 3; ++i){
        printf("%c\n", *(carrayPtr + i));
    }

    int* p;
    printf("un-initialized pointer address: %p\n", p);



}