
#include <stdio.h>


void reg_conv(){
    printf("Welcome to reg_conv()\n");
    while (1){
        char c = fgetc(stdin);
        if (c >= 65 && c <= 90)
            fputc(c + 32, stdout);
        else if (c == EOF)
            break;
        else if (c == '\n')
            printf("\n");
        else
        {
            fputc(c, stdout);
        }
    }
}

void plus_conv(int i){
    printf("Welcome to plus_conv()\n");
    while (1){
        char c = fgetc(stdin);
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


void minus_conv(int i){
    printf("Welcome to minus_conv()\n");
    while (1){
        char c = fgetc(stdin);
        if (c == EOF)
            break;
        else if (c == '\n')
            printf("\n");
        else
            fputc(c - i, stdout);
    }
}



