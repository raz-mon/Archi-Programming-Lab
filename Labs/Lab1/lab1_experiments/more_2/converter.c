

#include <stdio.h>


int main(int argc, char* argv[]){
    FILE* out = stdout;
    FILE* in = stdin;
    char c;
    while(1){
        c = fgetc(in);
        if (c >= 97 && c <= 122 || c == 32)
            fputc(c, out);
        else if (c >= 65 && c <= 90)
            fputc(c + 32, out);
        else if (c == EOF)
            break;
        else if (c == '\n')
            fputc('\n', out);
        else
            fprintf(out, "\nsorry, don't know that char at the moment.");
    }
    return 1;
}