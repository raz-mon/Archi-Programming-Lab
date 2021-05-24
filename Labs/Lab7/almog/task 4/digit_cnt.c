#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int digit_cnt(char * argv);

int main(int argc, char * argv[]){
    printf("%d\n" , digit_cnt(argv[1]));
    return 0;
}

int digit_cnt(char * argv){
    int counter = 0;
    //char c = numstr;
    int i = 0;
    while(argv[i] != 0){
        if (argv[i] >= '0' && argv[i] <= '9')
            counter++; 
        i++;
    }
    return counter;
}