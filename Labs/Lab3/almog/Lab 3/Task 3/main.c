#include <stdio.h>
#include <stdlib.h>

char * infected_file_name;

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

struct fun_desc {
  char *name;
  void (*fun)(link ** virus_list);
};


void load(link ** virus_list);    //Load the signatures from file to link list
void print(link ** virus_list);
void detect(link ** virus_list);
void eradicate(link ** virus_list);

virus* readVirus(FILE * file , char mode);
void detect_virus(char *buffer, unsigned int size, link * virus_list, FILE* output);
void kill_virus(char *fileName, int signitureOffset, int signitureSize);

void list_print(link *virus_list, FILE*); 
     /* Print the data of every link in list to the given stream. Each item followed by a newline character. */
link* list_append(link* virus_list, virus* data); 
     /* Add a new link with the given data to the list 
        (either at the end or the beginning, depending on what your TA tells you),
        and return a pointer to the list (i.e., the first link in the list).
        If the list is null - create a new entry and return a pointer to the entry. */
void list_free(link *virus_list); /* Free the memory allocated by the list. */


int main(int argc, char ** argv){
    infected_file_name = argv[1];
    struct fun_desc menu[] = { { "Load signatures", load }, { "Print signatures", print },
                             {"Detect viruses" , detect}, {"Fix file" , eradicate } , { NULL, NULL } };
    int size = sizeof(menu)/sizeof(menu[0]);
    link * first = calloc(2 , sizeof(link));
    
    while (1)
    {
        printf("Please choose a function:\n");
        for(int i = 0; i < size - 1; i++){
            printf("%d) %s\n" , i + 1 , menu[i].name);
        }
        printf("Option: ");
        char c = fgetc(stdin);
        char temp = fgetc(stdin);
        if(c  < 49 | c > 48 + size - 1){
            printf("Not within bounds\n");
            list_free(first);
            exit(0); 
        }
        else{
            printf("Within bounds\n");
            //free memory and allocate new
            if( c - 49 == 0){
                list_free(first);
                first = calloc(2 , sizeof(link));
            }

            menu[c - 49].fun(&first);
            printf("DONE.\n");
        }
        printf("\n");
    }
    return 0;
}