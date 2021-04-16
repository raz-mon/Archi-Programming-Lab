
#include <stdlib.h>
#include <stdio.h>


//void assFunc(int x);

void assFunc(int x);     // Untill we know how to print.
char check_validity(int x);

int main(int argc, char* argv[]){
    /*
    1. Prompts for and reads one integer (32 bits) number x in decimal base from the user (you may use fgets() and sscanf()).
    2. You may assume that the input is always valid.
    3. Calls 'void assFunc(int x)' written in assembly language with the above integer as an argument 
    */

    char temp[33];
    char c_num[32];
    fgets(temp, 33, stdin);
    //int x = atoi(c_num);
    int x = 0;
    sscanf(temp, "%d", &x);
    assFunc(x);
    printf("\n");
    
}

char check_validity(int x){
    printf("num: %d\n", x);
    if (x % 2 == 0)
        return 1;       // x is even -> return 1.
    else
        return 0;
}
