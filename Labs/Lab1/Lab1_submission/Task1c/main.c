

#include <stdio.h>
//#include <string.h>

void getParams(char* args, int argc, char** argv);
void reg_conv();
void plus_conv(int i);
void minus_conv(int i);

int main(int argc, char* argv[]){

    if (argc == 1)      // No command arguments.
        reg_conv();
    
    else if (argc > 1){     // Program has command arguments.
        int counter = 0;
        char par[3];
        getParams(par, argc, argv);
        int int_jump;

        // Transformation without sscanf:
        char c = par[2];
        if (c > 59)
            c = c - 55;
        else
            c = c - 48;
        
        int_jump = (int) c;

        if (par[0] == '+')
            plus_conv(int_jump);
        else
            minus_conv(int_jump);   
    }
    
    return 1;
    
}