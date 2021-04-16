

#include <stdio.h>

void converter(){
    FILE* out = stdout;
    FILE* in = stdin;
    char c;
    while(1){
        char c = fgetc(in);
        if (c >= 65 && c <= 90)
            fputc(c + 32, out);
        else if (c == EOF)
            break;
        else
            fputc(c, out);
    }
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
