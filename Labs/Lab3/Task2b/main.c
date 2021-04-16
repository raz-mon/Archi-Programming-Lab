
#include <stdio.h>
#include <stdlib.h>

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;


typedef struct link link;
 
struct link {
    link *nextVirus;
    virus *vir;
};

//menuStuff(fun_desc* men);
char bounds(int size, link** list);
int printHex(unsigned char* buffer, int length);
virus* readVirus(FILE* file);
void printVirus(virus* virus, FILE* output);
void list_free(link *virus_list);
link* list_append(link* virus_list, virus* data);
void list_print(link *virus_list, FILE*);

void loadSignatures(link** l);
void printSignatures(link** l);
void detect_virus_caller(link** l);
void fix_file(link** l);

void detect_virus(char *buffer, unsigned int size, link *virus_list, FILE* output);

struct fun_desc {       // Definition of the struct.
    char* name;
    void (*fun)(link** L);      // This is the pointer to the function, which gets a char as an argument.
};

//typedef struct fun_desc fun_desc;
struct fun_desc menu[] = { {"Load signatures", loadSignatures}, {"Print signatures", printSignatures}, {"detect viruses", detect_virus_caller},
                             {"Fix file", fix_file}, {NULL, NULL} };

char* file_name;

int main(int argc, char* argv[]){

    file_name = argv[1];
    
    link* list = NULL;

    int size = sizeof(menu) / sizeof(struct fun_desc) - 1;      // Can also use "strlen()". But understand the difference and then decide.

    while(1){

        // link that will hold the viruss' of the given file.
        
        // Menu shit:
        // Ask the user to kindly input some data.
        printf("%s", "please choose a function:\n");
        //compute the bounds of the wanted input.
        
        // Print the menu's content (options).
        for (int i = 0; i < size; ++i){
            printf("%d) %s\n", i+1, menu[i].name);
        }
        // Get choice from user, and see if it's in-bounds.
        printf("%s", "option: ");
        char inChar = bounds(size, &list);     // print "within bounds" or "not within bounds" appropreatly, and place the given char given by the user in "inChar". 
            
        menu[inChar-49].fun(&list);      // 0 === 48. (-1) because we want to start a '1' not '0' at the displayed menu.
    }
    free(list);

    return 1;
}