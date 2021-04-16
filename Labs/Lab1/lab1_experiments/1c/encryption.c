


#include <stdio.h>
//#include <string.h>

void getParams(char* args, int argc, char** argv);
void regconv();
void plus_conv(int i);
void minus_conv(int i);

int main(int argc, char* argv[]){
    if (argc > 1){
        int counter = 0;
        char par[3];
        getParams(par, argc, argv);
 //       fprintf(stderr, par);
 //       fprintf(stderr, "\n");
        int int_jump;
        // transfrom par[2] to an int:
        sscanf(&par[2], "%x", &int_jump);           // Make sure you can use this function!!
    
        if (argc == 1)
            reg_conv();
        else if (par[0] == '+')
            plus_conv(int_jump);
        else
            minus_conv(int_jump);
    
    }
    return 1;
    
}