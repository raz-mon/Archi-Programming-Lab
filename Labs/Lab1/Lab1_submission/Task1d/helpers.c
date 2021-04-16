
#include <stdio.h>

void converter(){
    printf("Welcome to reg_conv()\n");
    while (1){
        char c = fgetc(stdin);
        if (c >= 65 && c <= 90)
            fputc(c + 32, stdout);
        else if (c == EOF)
            break;
        else
        {
            fputc(c, stdout);
        }
    }
}

void converter2(FILE* path){
    printf("Welcome to reg_conv2()\n");
    while (1){
        char c = fgetc(path);
        if (c >= 65 && c <= 90)
            fputc(c + 32, stdout);
        else if (c == EOF)
            break;
        else
        {
            fputc(c, stdout);
        }
    }
}

int check_if_file(char* argv_1){
    if (argv_1[1] == 'e')
        return 0;
    return 1;
}


void conv_w_debug(){
    
    int counter = 0;
    while (1){
        char c = fgetc(stdin);
        if (c >= 65 && c <= 90){
            fprintf(stderr, "%d %d\n", c, c + 32);
            c = c + 32;
            counter++;
        }
        else if (c == EOF)
            break;
        else if (c == '\n'){
            fprintf(stderr, "\nnumber of letters converted: %d\n\n", counter);
            counter = 0;
        }
        else
        {
            fprintf(stderr, "%d %d\n", c, c);
        }
        fputc(c, stdout);
    }
}


void plus_conv(int i, FILE* path){
    printf("Welcome to plus_conv()\n");
    while (1){
        char c = fgetc(path);
        if (c == EOF)
            break;
        else if (c == '\n')
            printf("\n");
        else
        {
            fputc(c + i, stdout);
        }
    }
}


void minus_conv(int i, FILE* path){
    printf("Welcome to minus_conv()\n");
    while (1){
        char c = fgetc(path);
        if (c == EOF)
            break;
        else if (c == '\n')
            printf("\n");
        else
            fputc(c - i, stdout);
    }
}


#include <stdio.h>
#include <string.h>

void getParams(char* args,int argc,char** argv){
    for (int i = 0; i < strlen(argv[1]); ++i){
        args[i] = argv[1][i];
    }
}








