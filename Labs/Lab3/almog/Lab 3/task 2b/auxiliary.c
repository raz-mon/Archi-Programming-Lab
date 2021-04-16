#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

char * infected_file_name;

void load(link ** virus_list);    //Load the signatures from file to link list
void print(link ** virus_list);
void detect(link ** virus_list);
void eradicate(link ** virus_list);

int strToint();

virus* readVirus(FILE * file);
void kill_virus(char *fileName, int signitureOffset, int signitureSize);
void detect_virus(char *buffer, unsigned int size, link * virus_list, FILE* output);
void list_print(link *virus_list, FILE*); 
     /* Print the data of every link in list to the given stream. Each item followed by a newline character. */
link* list_append(link* virus_list, virus* data); 
     /* Add a new link with the given data to the list 
        (either at the end or the beginning, depending on what your TA tells you),
        and return a pointer to the list (i.e., the first link in the list).
        If the list is null - create a new entry and return a pointer to the entry. */
void list_free(link *virus_list); /* Free the memory allocated by the list. */

virus* readVirus(FILE * file){
    virus * v = malloc(sizeof(virus));
    fread(v, sizeof(char) , 18 , file);
    (*v).sig = (unsigned char *)malloc(sizeof(char) * (*v).SigSize);
    fread(v->sig, sizeof(char) , (*v).SigSize , file);
    return v;
}

void load(link ** virus_list){
    char  temp[100];
    char args[100];
    printf("Write signatures file name: ");
    fgets(temp,100,stdin);
    sscanf(temp, "%s" , args);
    if(args[0] == '\0'){return;}

    FILE * file = fopen(args , "r");

    fseek(file,0,SEEK_END);
    int file_size = ftell(file);

    fseek(file,4,SEEK_SET);
    while (ftell(file) < file_size)
    {
        virus * v = readVirus(file);
        *virus_list = list_append(*virus_list , v);
    }
    fclose(file);
}


void print(link ** virus_list){   list_print(*virus_list , stdout); }

void list_print(link *virus_list, FILE* file){
    if(virus_list->vir != NULL){
        if (virus_list->nextVirus != NULL){list_print(virus_list->nextVirus,file);}
        fprintf(file, "Virus size: %d\nVirus name: %s\nVirus signature:\n", virus_list->vir->SigSize , virus_list->vir->virusName);
        for(int i = 0; i < virus_list->vir->SigSize; ++i){
            fprintf(file, "%02X " , virus_list->vir->sig[i]);
        }
        fprintf(file,"\n\n");
    }
}

link* list_append(link* virus_list, virus* data){
    link * new_first;
    if(virus_list->vir == NULL){
        virus_list->vir = data;
        new_first = virus_list;
    }
    else
    {
        new_first = calloc(2 , sizeof(link));
        new_first->vir = data;
        new_first->nextVirus = virus_list;
    }
    return new_first;
}

void list_free(link *virus_list){
    if(virus_list->vir != NULL){
        free(virus_list->vir->sig);
        free(virus_list->vir);
    }
    if(virus_list->nextVirus != NULL){ list_free(virus_list->nextVirus); }
    free(virus_list);
}

void detect(link ** virus_list){
    char buffer[10000];

    FILE * file = fopen(infected_file_name,"r");
    fseek(file,0,SEEK_END);
    int file_size = ftell(file);
    fseek(file,0,SEEK_SET);
    fread(buffer,1,file_size,file);

    int size;
    if(sizeof(buffer) < file_size){ size = file_size; }
    else size = sizeof(buffer);
    
    detect_virus(buffer,size, *virus_list,stdout);
    fclose(file);
}

void detect_virus(char *buffer, unsigned int size, link * virus_list, FILE* output){
    while (virus_list->vir != NULL)
    {
        int vir_size = virus_list->vir->SigSize;
        char * curr_pos = buffer;
        for(int i = 0; i < size - vir_size; i++){
            if(memcmp(virus_list->vir->sig ,curr_pos + i ,vir_size) == 0){
                printf("Virus found at: 0x%x hex / %d numerical\nVirus name: %s\nVirus size: %d\n",i,i, virus_list->vir->virusName, vir_size);
                break;
            }
        }
        if(virus_list->nextVirus != NULL){ virus_list = virus_list->nextVirus; }
        else return;
    }
}

void eradicate(link ** virus_list){
    printf("\nPlease enter byte location: ");
    int offset = strToint();

    printf("\nPlease enter virus size: ");
    int size = strToint();
    kill_virus(infected_file_name,offset,size);
}

int strToint(){
    char temp[100];
    char str[100];
    fgets(temp,100,stdin);
    sscanf(temp,"%s",str);
    return atoi(str);
}

void kill_virus(char *fileName, int signitureOffset, int signitureSize){
    FILE * file = fopen(infected_file_name,"r+");
    fseek(file,signitureOffset,SEEK_SET);
    unsigned char changedCode[signitureSize];
    for(int i = 0; i < signitureSize; i++){
        changedCode[i] = 0x90;         // Representing code for 0x90 - NOP in decimal
    }
    fwrite(&changedCode,sizeof(unsigned char),signitureSize,file);
    fclose(file);
}