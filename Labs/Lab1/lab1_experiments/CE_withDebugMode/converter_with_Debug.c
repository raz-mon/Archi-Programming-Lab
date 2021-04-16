


#include <stdio.h>
//#include <string.h>

void getParams(char* args, int argc, char** argv);
void conv();
void conv_w_debug();

int main(int argc, char* argv[]){
    if (argc > 1){
        int counter = 0;
        char par[2];
        getParams(par, argc, argv);
        fprintf(stderr, par);
        fprintf(stderr, "\n");
        conv_w_debug();
    }
    else if (argc == 1){
        conv();
    }

    return 1;
    
}