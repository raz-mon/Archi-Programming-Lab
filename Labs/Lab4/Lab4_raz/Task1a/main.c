#include "./lab4_util.h"

int system_call();

int main(int argc, char* argv[], char* envp[]){
    
    char c[1];
    while(1){
        system_call(3, 0, c, 1);
        if (*c >= 65 && *c <= 90){
            *c = *c + 32;
            system_call(4, 1, c, 1);
        }
        else if (*c == 0)
            break;
        else
            system_call(4, 1, c, 1);
    }
    return 1;
}