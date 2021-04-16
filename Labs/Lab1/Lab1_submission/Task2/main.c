


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
                plus_conv(int_jump, stdout);
            else
                minus_conv(int_jump, stdout);
        }
        else{       // '1' -> is a file path.
            FILE* f = fopen(argv[1] + 2, "w+");
            converter2(f);
            fclose(f);
            printf("\n");
        }
    }
    else if (argc == 3){
        FILE* f = fopen(argv[2] + 2, "w+");
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
    }
    return 1;
}







/*

    //code for file-to-file conversion
    FILE* output = fopen("output", "w+");
    FILE* input = fopen("input", "r");
    converter3(input, output);

    fclose(output);
    fclose(input);
*/

