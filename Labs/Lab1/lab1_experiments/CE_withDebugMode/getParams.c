

#include <stdio.h>
#include <string.h>

void getParams(char* args,int argc,char** argv){
    for (int i = 0; i < strlen(argv[1]); ++i){
        args[i] = argv[1][i];
    }
//    args = argv[1];    
    
}