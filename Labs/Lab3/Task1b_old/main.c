
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
char bounds(int size);
int printHex(unsigned char* buffer, int length);
virus* readVirus(FILE* file);
void printVirus(virus* virus, FILE* output);
void list_free(link *virus_list);
void list_print(link *virus_list, FILE*);
link* list_append(link* virus_list, virus* data);
link* loadSignatures(link* l);
link* printSignatures(link* l);
link* detectViruses(link* list);

struct fun_desc {       // Definition of the struct.
    char* name;
    link* (*fun)(link* L);      // This is the pointer to the function, which gets a char as an argument.
};

//typedef struct fun_desc fun_desc;
struct fun_desc menu[] = { {"Load signatures", loadSignatures}, {"Print signatures", printSignatures}, {"detect viruses", detectViruses}, {NULL, NULL} };

int main(int argc, char* argv[]){
    
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
        char inChar = bounds(size);     // print "within bounds" or "not within bounds" appropreatly, and place the given char in "inChar".        
        if (inChar == 49)
            list = menu[inChar-49].fun(list);      // 0 === 48. (-1) because we want to start a '1' not '0' at the displayed menu.
        else
            menu[inChar - 49].fun(list);
    }
    list_free(list);
    return 1;
}











/*
        //  read all viruss' from signatues-L file.
    FILE* file = fopen("signatures-L", "r");
    fseek(file, 0, SEEK_END);
    int end = ftell(file);
    fseek(file, 4, SEEK_SET);       // put the offset after 4 first bytes (irrelevant right now).
    do{
        virus* v = readVirus(file);
        list = list_append(list, v);
        printVirus(v, stdout);
//        free(v->sig);
//        free(v);
    } while (ftell(file) != end);

    list_print(list, stdout);

    list_free(list);
*/
    








