

#include <stdio.h>

int main(int argc, char* argv[]){
    FILE* in = stdin;
    FILE* out = stdout;
    while(1){
        char c = fgetc(in);
        if (c >= 65 && c <= 90)
            fputc(c + 32, out);
        else if (c == EOF)
            break;
        else
            fputc(c, out);
    }
    return 1;
}