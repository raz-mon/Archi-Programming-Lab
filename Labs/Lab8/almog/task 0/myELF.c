#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define  NAME_LEN  128
#define  BUF_SZ    10000

typedef struct {
    char debug_mode;
    char file_name[NAME_LEN];
    int unit_size;
    unsigned char mem_buf[BUF_SZ];
    size_t mem_count;

    //Lab 8
    int Currentfd;
    void* map_start;
    struct stat fd_stat;
    Elf32_Ehdr * header;
    int num_of_section_headers;
    

} state;


struct fun_desc {       // Definition of the struct.
    char* name;
    void (*fun)(state* s);      // This is the pointer to the function, which gets a char as an argument.
};

void debug_mode(state* s);
void exm_elf_file(state* s);
void print_sec_names(state* s);
void print_sym(state* s);
void rel_tab(state* s);
void quit(state* s);


int main(int argc, char ** argv){
    
    struct fun_desc menu[] = { {"Toggle Debug Mode", debug_mode }, { "Examine ELF File", exm_elf_file },
                             {"Print Section Names" , print_sec_names}, {"Print Symbols" , print_sym } ,
                             { "Relocation Tables", rel_tab }, { "Quit", quit }, { NULL, NULL } };
    int size = sizeof(menu)/sizeof(menu[0]);

    state* current_state = calloc(1 , sizeof(state));
    current_state->debug_mode = 0;
    current_state->unit_size = 1;
    current_state->mem_count = 0;
    current_state->Currentfd = -1;
    sscanf("", "%s", current_state->file_name);
    
    while (1)
    {
        if(current_state->debug_mode == 1){
            printf("========================================================\n\n");
            printf("Unit Size: %d   |   File name: %s   |   Memory count: %d\n\n" ,
            current_state->unit_size , current_state->file_name, current_state->mem_count);
            printf("========================================================\n\n");
        }
        printf("Choose action:\n");
        for(int i = 0; i < size - 1; i++){
            printf("%d) %s\n" , i , menu[i].name);
        }
        printf("Option: ");
        char c = fgetc(stdin);
        char temp = fgetc(stdin);
        if(c  < 48 | c > 48 + size - 1){
            printf("Not within bounds\n");
            free(current_state);
            exit(0); 
        }
        else{
            printf("Within bounds\n");
            //free memory and allocate new

            menu[c - 48].fun(current_state);
            printf("DONE.\n");
        }
        printf("\n");
    }
    return 0;
}