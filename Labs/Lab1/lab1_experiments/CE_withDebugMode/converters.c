

#include <stdio.h>

void conv(){
    while (1){
        char c = fgetc(stdin);
        //if (c >= 97 && c <= 122 || c == 32)
          //  fputc(c, stdout);
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

void conv_w_debug(){            // Code doesn't work!! Need to use memory on the Heap!!!!!
    
    int counter = 0;
    char encrypted[256];
    //char* encrypted;
    int size = 0;

    while (1){
        char c = fgetc(stdin);
        if (c >= 65 && c <= 90){
            printf("%d %d", c, c + 32);
            size++;
            //encrypted = addchar(encrypted, c + 32);
            if (size < 256)
                encrypted[size-1] = c + 32;
            /*
            char temp[size];
            for (int i = 0; i < size-1; ++i){
                temp[i] = encrypted[i];
            }
            temp[size-1] = c + 32;
            encrypted = temp;
            */
            counter++;
        }
        else if (c == EOF)
            break;
        else if (c == '\n'){
            printf("\nnumber of letters converted: %d\n\n", counter);
            printf("%s\n", encrypted);
            printf("\nPlease enter your sentence:");
            counter = 0;
            for (int i=0; i < size; ++i){
                encrypted[i] = NULL;
            }
            size = 0;
        }
        else
        {
            printf("%d %d", c, c);
            size++;
            if (size < 256)
                encrypted[size-1] = c;
            /*
            char temp[size];
            for (int i = 0; i < size-1; ++i){
                temp[i] = encrypted[i];
            }
            temp[size-1] = c + 32;
            char* encrypted = temp;
            */
        }
        fprintf(stdout, "\n");
    }
}

//char* addchar(char* encrypted, char c){

//}






















