
#include <stdio.h>


int* q;
int* t = 456646;

int main(int argc, char* argv[]){
    int iarray[] = {1,2,3};
    char carray[] = {'a','b','c'};
    int* iarrayPtr;
    char* carrayPtr;

    // Initialize iarrayptr to the address of the first element of iarray, and iterate through it by adding "1" each time. (Could have used strlen here).
    iarrayPtr = iarray;
    for(int i = 0; i < 3; ++i){
        printf("%d ", *(iarrayPtr + i));
    }
    printf("%s", "\n");

    // Initialize carrayptr to the address of the first element of carray, and iterate through it by adding "1" each time. (Could have used strlen here).
    carrayPtr = carray;
    for(int i = 0; i < 3; ++i){
        printf("%c ", *(carrayPtr + i));
    }
    printf("%s", "\n");

    int* p;
    printf("un-initialized pointer address: %p\n\n\n\n\n\n", &p);

    printf("un-initialized global pointer address: %p\n", &q);

    printf("initialized global pointer address: %p\n", &t);

     printf("command-line argument pointer address: %p\n", &argv[0][0]);
}