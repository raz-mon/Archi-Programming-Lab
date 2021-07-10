#include <stdio.h>
#include <string.h>


int digit_cnt(char* numstr){

    int counter = 0;
    //char c = numstr;
    int i = 0;
    while(numstr[i] != 0){
        if (numstr[i] >= '0' && numstr[i] <= '9')
            counter++; 
        i++;
    }
    return counter;
/*
    for (int i = 0; i < 5; i++){
        if (numstr[i] >= '0' && numstr[i] <= '9')
        counter++; 
    }
    return counter;
*/
    
}
// 4ed 80
int main(int argc, char* argv[]){

    digit_cnt(argv[1]);
    return 0;
    
}

/*
int digit_cnt(int argc, char* argv[]){
    if (argc > 1){
        int counter = 0;
        for (int i = 0; i < strlen(argv[1]); i++){
            if (argv[1][i] >= '0' && argv[1][i] <= '9')
            counter++; 
        }
        printf("%d\n", counter);
        return counter;
    }
    return 0;
}
*/


/*



*/