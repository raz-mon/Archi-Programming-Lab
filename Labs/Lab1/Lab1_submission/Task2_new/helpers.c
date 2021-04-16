
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

void converter2_in(FILE* path){
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

void converter2_out(FILE* path){
    printf("Welcome to reg_conv2()\n");
    while (1){
        char c = fgetc(stdin);
        if (c >= 65 && c <= 90)
            fputc(c + 32, path);
        else if (c == EOF)
            break;
        else
        {
            fputc(c, path);
        }
    }
}

int check_if_file(char* argv_1){
    if (argv_1[1] == 'e')
        return 0;           // 0 -> encryption
    return 1;               // 1 -> file
}

void plus_conv_in(int i, FILE* path){
    printf("Welcome to plus_conv()\n");
    while (1){
        char c = fgetc(path);
        if (c == EOF)
            break;
        else if (c == '\n')
            fputc(c, stdout);
        else
        {
            fputc(c + i, stdout);
        }
    }
}

void plus_conv_out(int i, FILE* path){
    printf("Welcome to plus_conv()\n");
    while (1){
        char c = fgetc(stdin);
        if (c == EOF)
            break;
        else if (c == '\n')
            fputc(c, path);
        else
        {
            fputc(c + i, path);
        }
    }
}

void minus_conv_in(int i, FILE* path){
    printf("Welcome to minus_conv()\n");
    while (1){
        char c = fgetc(path);
        if (c == EOF)
            break;
        else if (c == '\n')
            fputc(c, stdout);
        else
            fputc(c - i, stdout);
    }
}

void minus_conv_out(int i, FILE* path){
    printf("Welcome to minus_conv()\n");
    while (1){
        char c = fgetc(stdin);
        if (c == EOF)
            break;
        else if (c == '\n')
            fputc(c, path);
        else
            fputc(c - i, path);
    }
}

#include <stdio.h>
#include <string.h>

void getParams(char* args,int argc,char** argv){
    for (int i = 0; i < strlen(argv[1]); ++i){
        args[i] = argv[1][i];
    }
}






//Bonus - from one file to another!
void converter3(FILE* in, FILE* out){
    printf("Welcome to reg_conv2()\n");
    while (1){
        char c = fgetc(in);
        if (c >= 65 && c <= 90)
            fputc(c + 32, out);
        else if (c == EOF)
            break;
        else
        {
            fputc(c, out);
        }
    }
}











































void conv_w_debug_plus(int i, FILE* path){
    int counter = 0;
    while (1){
        char c = fgetc(stdin);
        if (c == EOF)
            break;
        else if (c >= 65 && c <= 90){
            fprintf(stderr, "%d %d\n", c, c + i);
            c = c + i;
            counter++;
        }
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













/*
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
*/







