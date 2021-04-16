


#include <stdio.h>


int main(int argc, char* arg[]){
    while (1){
        char c = fgetc(stdin);
        if (c >= 97 && c <= 122 || c == 32)
            fputc(c, stdout);
        else if (c >= 65 && c <= 90)
            fputc(c + 32, stdout);
        else if (c == EOF)
            break;
        else if (c == '\n')
            printf("\n");
        else
        {
            fputs("Don't know that char mister!!", stdout);
        }
    }
    
}