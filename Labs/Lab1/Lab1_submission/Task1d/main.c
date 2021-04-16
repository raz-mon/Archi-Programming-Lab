


#include <stdio.h>
#include <string.h>

void converter();
void converter2(FILE* path);
int check_if_file(char* argv_1);
void minus_conv(int i, FILE* path);
void plus_conv(int i, FILE* path);
void getParams(char* args,int argc,char** argv);

int main(int argc, char* argv[]){

    // Check the command line arguments:

    if (argc == 1)
        converter();
    else if (argc == 2){
        if (check_if_file(argv[1]) == 0){       // '0' if this is an encryption key.
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
                plus_conv(int_jump, stdin);
            else
                minus_conv(int_jump, stdin);
        }
        else{       // '1' -> is a file path.
            FILE* f = fopen(argv[1] + 2, "r");
            converter2(f);
            fclose(f);
            printf("\n");
        }
    }
    else if (argc == 3){
        FILE* f = fopen(argv[2] + 2, "r");
        // regular program, with f instead of stdin:
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
                plus_conv(int_jump, f);
            else
                minus_conv(int_jump, f);
            printf("\n");
    }
    return 1;
}



/*
    if (argc == 1)
        converter();
    else if (argc == 3){
        
        //get input path: 
        char* p = argv[2];
        // get the relevant part:
        char c = 1;
        char path[1086];
        int i = 2;
        while (c != 0){ 
            path[i - 2] = p[i];
            i++;
            c = p[i];
        }

        char par[3];
        getParams(par, argc, argv);
        int int_jump;

        // Transformation without sscanf:
        char d = par[2];
        if (d > 59)
            d = d - 55;
        else
            d = d - 48;
        
        int_jump = (int) d;

        if (par[0] == '+')
            plus_conv(int_jump, path);
        else
            minus_conv(int_jump, path); 
    }
    */
