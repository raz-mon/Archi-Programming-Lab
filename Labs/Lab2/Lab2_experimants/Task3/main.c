#include <stdlib.h>
#include <stdio.h>
#include <string.h>


char censor(char c) {
  if(c == '!')
    return '.';
  else
    return c;
}

char encrypt(char c){
    if (c >= 32 && c <= 126){
        return c + 3;
    }
    else
        return c;
}

char decrypt(char c){
    if (c >= 32 && c <= 126){
        return c - 3;
    }
    else
        return c;
}

char my_get(char c){
    return fgetc(stdin);
}

char cprt(char c){
 //   printf("character int value: %d\n", c);
    if (c >= 32 && c <= 126){
        fputc(c, stdout);
    }
    else
        fputc('.', stdout);
    fputc('\n', stdout);
    return c;
}

char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  /* TODO: Complete during task 2.a */
  for (int i = 0; i < array_length; ++i){
      mapped_array[i] = f(array[i]);
  }
  //free(array);
  return mapped_array;
}
 
int main(int argc, char **argv){
    
    char *carray = (char *)malloc(5 * sizeof(char));        // Empty string of 5 bytes.

    struct fun_desc {       // Definition of the struct.
        char* name;
        char (*fun)(char);      // This is the pointer to the function, which gets a char as an argument.
    };

    // Initialize the menu (in-decleration).
    struct fun_desc menu[] = { {"Censor", censor}, {"Encrypt", encrypt}, {"Decrypt", decrypt}, {"Print String",cprt}, {"Get String", my_get}, {NULL, NULL}};

    while(1){

        // Ask the user to kindly input some data.
        printf("%s", "please choose a function:\n");

        //compute the bounds of the wanted input.
        int size = sizeof(menu) / sizeof(struct fun_desc) - 1;      // Can also use "strlen()". But understand the difference and then decide.

        // Print the menu's content (options).
        for (int i = 0; i < size; ++i){
            printf("%d) %s\n", i, menu[i].name);
        }

        // Get choice from user, and see if it's in-bounds.
        printf("%s", "option: ");
        char inChar = fgetc(stdin);
        char temp = fgetc(stdin);       // Just to get rid of the '\n' that we get when the user hit's enter.
        
        if (inChar < 48 || inChar > 48 + size - 1){
            printf("%s", "Not within bounds\n");
            exit(0);
        }
        else
            printf("%s", "within bounds\n");
        
        char* temp2 = map(carray, 5, menu[inChar - 48].fun);
        free(carray);
        carray = temp2;
        
        
        }
}



 

