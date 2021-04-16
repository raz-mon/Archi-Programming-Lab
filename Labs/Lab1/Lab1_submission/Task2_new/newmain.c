


#include <stdio.h>
#include <string.h>

void converter();
void converter2_in(FILE* path);
void converter2_out(FILE* path);
int check_if_file(char* argv_1);
void minus_conv_in(int i, FILE* path);
void minus_conv_out(int i, FILE* path);
void plus_conv_in(int i, FILE* path);
void plus_conv_out(int i, FILE* path);
void getParams(char* args,int argc,char** argv);

int main(int argc, char* argv[]){

    // Check the command line arguments:
    if (argc == 1)
        converter();
    else if (argc == 2){
        if (check_if_file(argv[1]) == 0){       // '0' if this is an encryption key.
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
                plus_conv_out(int_jump, stdout);
            else
                minus_conv_out(int_jump, stdout);
        }
        else{       // '1' -> is a file path.
            if (argv[1][1] == 'o'){
                FILE* f = fopen(argv[1] + 2, "w+");
                converter2_out(f);
                fclose(f);
                printf("\n");
            }
            else{
                FILE* f = fopen(argv[1] + 2, "r");
                converter2_in(f);
                fclose(f);
                printf("\n");
            }
        }
    }
    else if (argc == 3){

        if (check_if_file(argv[1]) == 0){       // first argument is an encryption key, second is output file path.

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


            FILE* f;
            if (argv[2][1] == 'o'){
                f = fopen(argv[2] + 2, "w+");

                if (par[0] == '+')
                plus_conv_out(int_jump, f);
                else
                minus_conv_out(int_jump, f);

                fclose(f);
                printf("\n");
            }
            else{
                f = fopen(argv[2] + 2, "r");

                if (par[0] == '+')
                plus_conv_in(int_jump, f);
                else
                minus_conv_in(int_jump, f);

                fclose(f);
                printf("\n");
            }
            
            
        }

        else{                                   // first argument is an output file path, second is an encryption key.


            char par[3];
            getParams(par, argc, argv + 1);
            int int_jump;
            // Transformation without sscanf:
            char c = par[2];
            if (c > 59)
                c = c - 55;
            else
                c = c - 48;
            int_jump = (int) c;


            FILE* f;
            if (argv[1][1] == 'o'){
                f = fopen(argv[1] + 2, "w+");

                if (par[0] == '+')
                plus_conv_out(int_jump, f);
                else
                minus_conv_out(int_jump, f);

                fclose(f);
                printf("\n");
            }
            else{
                f = fopen(argv[1] + 2, "r");

                if (par[0] == '+')
                plus_conv_in(int_jump, f);
                else
                minus_conv_in(int_jump, f);

                fclose(f);
                printf("\n");
            }
        }
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

