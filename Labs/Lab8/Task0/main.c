#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h> // Prototype for basename() function
#include <sys/types.h>
#include <unistd.h>
#include <elf.h>
#include <fcntl.h>
#include <sys/mman.h>

#define NAME_LEN 128

typedef struct {
    char debug_mode;
    char file_name[NAME_LEN];
    int Currentfd;
    int st_size;
    void* map_start;
    struct stat fd_stat;
    Elf32_Ehdr * header;
    //int num_of_section_headers;
} state;

struct fun_desc {       // Definition of the struct.
        char* name;
        void (*fun)(state*);      // This is the pointer to the function, which gets a state as an argument.
};

state s_init = {0, "", -1, 0, 0, 0, 0};
state* s = &s_init;

void quit(state* s);
void TogDMode(state* s);
void examine_ELF_FILE(state* s);
void print_section_names(state* s);
void print_symbols(state* s);
void relocation_tables(state* s);

int main(int argc, char* argv[]){

    struct fun_desc menu[] = { {"Toggle Debug Mode", TogDMode}, {"Examine ELF File", examine_ELF_FILE},
     {"Print Section Names", print_section_names}, {"Print Symbols", print_symbols}, {"Relocation Tables", relocation_tables},
       {"Quit", quit},{NULL, NULL}};

    while(1){

        // Ask the user to kindly input some data.
        printf("choose action:\n");

        //compute the bounds of the wanted input.
        int size = sizeof(menu) / sizeof(struct fun_desc) - 1;      // Can also use "strlen()". But understand the difference and then decide.

        // If debug mode is on, print the unit_size, file_name, and mem_count.
        
        if (s->debug_mode == 1){
            //printf("unit size: %d\n", s->unit_size);
            printf("file name: %s\n", s->file_name);
            //printf("mem count: %d\n", s->mem_count);    
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