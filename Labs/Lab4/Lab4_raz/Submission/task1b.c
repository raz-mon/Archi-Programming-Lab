#include "./lab4_util.h"

int system_call();
void encoder();
void encoder_in(char* filename);
void encoder_out(char* filename);
void encoder_in_and_out(char* in_name, char* out_name);

int main(int argc, char* argv[], char* envp[]){
    int i;
    if (argc == 1){
        encoder();
        system_call(4, 1, "\n", 1);
    }
    else if (argc == 3){
        char* in;
        char* out;
        for (i = 1; i < argc; ++i){
            char starter = argv[i][1];
            switch(starter) 
            {
                case 'i':
                    in = argv[i]+2;
                    break;
                case 'o':
                    out = argv[i]+2;
                    break;
            }
        }
        encoder_in_and_out(in, out);
    }
    else{
        for (i = 1; i < argc; ++i){
            char starter = argv[i][1];
            switch(starter) 
            {
                case 'i':
                    encoder_in(argv[i]+2);
                    break;
                case 'o':
                    encoder_out(argv[i]+2);
                    break;
            }
        }
        system_call(4, 1, "\n", 1);
    }
    
    return 0;
}