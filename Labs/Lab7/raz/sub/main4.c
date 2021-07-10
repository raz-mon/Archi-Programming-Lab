#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define  NAME_LEN  128
#define  BUF_SZ    10000

typedef struct {
  char debug_mode;
  char file_name[NAME_LEN];
  int unit_size;
  unsigned char mem_buf[BUF_SZ];
  size_t mem_count;
  /*
   .
   .
   Any additional fields you see necessary
  */
} state;

struct fun_desc {       // Definition of the struct.
        char* name;
        void (*fun)(state*);      // This is the pointer to the function, which gets a state as an argument.
    };

void TogDMode(state* s);
void SetFileName(state* s);
void SetUnitSize(state* s);
void LoadIntoMem(state* s);
void memDisp(state* s);
void modifyMem(state* s);
void saveToFile(state* s);
void quit(state* s);

char fn[NAME_LEN];
state s_init = {0, "", 1, "", 0};
state* s = &s_init;

int main(int argc, char* argv[]){

    struct fun_desc menu[] = { {"Toggle Debug Mode", TogDMode}, {"Set File Name", SetFileName},
     {"Set Unit Size", SetUnitSize}, {"Load Into Memory",LoadIntoMem}, {"Memory Display", memDisp},
      {"Save Into File", saveToFile}, {"Memory Modify", modifyMem},
       {"Quit", quit},{NULL, NULL}};

    while(1){

        // Ask the user to kindly input some data.
        printf("choose action:\n");

        //compute the bounds of the wanted input.
        int size = sizeof(menu) / sizeof(struct fun_desc) - 1;      // Can also use "strlen()". But understand the difference and then decide.

        // If debug mode is on, print the unit_size, file_name, and mem_count.
        
        if (s->debug_mode == 1){
            printf("unit size: %d\n", s->unit_size);
            printf("file name: %s\n", s->file_name);
            printf("mem count: %d\n", s->mem_count);    
        }
        
        // Print the menu's content (options).
        for (int i = 0; i < size; ++i){
            printf("%d) %s\n", i, menu[i].name);
        }

        // Get choice from user, and see if it's in-bounds.
        printf("%s", "option: ");
        
        // Assign the input char by user to 'inChar'.
        char inChar;
        char temp[50];
        fgets(temp, 50, stdin);
        sscanf(temp, "%c", &inChar);
        
        // Check if 'inChar' is within bounds or not.
        if (inChar < 48 || inChar > 48 + size - 1){
            printf("%s", "Not within bounds\n");
            exit(0);
        }
        else
            printf("%s", "within bounds\n");
        
        // If it is, perform the relevant function.
        menu[inChar - 48].fun(s);      // s is a state. probably clearer soon..  
        printf("\n");
        }

}
