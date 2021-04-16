

#include <stdlib.h>
#include <stdio.h>

// typedef struct fun_desc fun_desc;

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

int printHex(unsigned char* buffer, int length){
    int printed = 0;
    for (int i = 0; i < length; ++i){
        printf("%02X ", buffer[i]);
        printed++;
    }
    return printed;
}

virus* readVirus(FILE* file){
    virus* Virus = (virus *)malloc(sizeof(virus));
    fread(Virus, sizeof(char), 18, file);
    Virus->sig = (unsigned char *) malloc(sizeof(char) * Virus->SigSize);
    fread(Virus->sig, sizeof(char), Virus->SigSize, file);
    return Virus;
}

void printVirus(virus* virus, FILE* output){
    printf("Virus name: %s\n", virus->virusName);
    printf("Virus size: %d\nsignature:\n", virus->SigSize);
    //printf("signature:\n%x\n", virus->sig);
    printHex(virus->sig, virus->SigSize);
    printf("%s", "\n");
}


 /* Print the data of every link in list to the given stream. Each item followed by a newline character. */
void list_print(link *virus_list, FILE* file){
    printVirus(virus_list->vir, file);
    if (virus_list->nextVirus != NULL)
        list_print(virus_list->nextVirus, file);
}
    
    
/* Add a new link with the given data to the list (either at the end or the beginning, depending on what your TA tells you),
        and return a pointer to the list (i.e., the first link in the list). If the list is null - create a new entry and return a pointer to the entry. */
link* list_append(link* virus_list, virus* data){
    if (virus_list == NULL){        // virus_list is empty.
        link* list = (link *)malloc(sizeof(link));
        list->vir = data;
        list->nextVirus = NULL;
        return list;
    }
    else{       // virus_list is not empty
        link* newLink = (link *)malloc(sizeof(link));
        newLink->nextVirus = virus_list;
        newLink->vir = data;
        return newLink;
    }
}
     
/* Free the memory allocated by the list. */
void list_free(link *virus_list){
    if (virus_list->vir != NULL)
        free(virus_list->vir);
    if (virus_list->nextVirus != NULL)
        list_free(virus_list->nextVirus);
} 

link* loadSignatures(link* list){

    if (list != NULL)
        list_free(list);

    // get the file name and assign it to "fileName" variable.
    char* fileName;
    char temp[20];
    fgets(temp, sizeof(temp), stdin);
    sscanf(temp, "%s", fileName);
    // load the file's content to list.
    FILE* file = fopen(fileName, "r");
    fseek(file, 0, SEEK_END);
    int end = ftell(file);
    list = (link *)malloc(sizeof(link));
    list->nextVirus = NULL;
    list->vir = NULL;
    fseek(file, 4, SEEK_SET);       // put the offset after 4 first bytes (irrelevant right now).
    do{
    virus* v = readVirus(file);
    list = list_append(list, v);
//   free(v->sig);
//   free(v);
    } while (ftell(file) != end);       
//    fclose(file);  
    return list;
}

link* printSignatures(link* list){
    link* temp = list;
    if (temp->vir != NULL)
        printVirus(temp->vir, stdout);
    if (temp->nextVirus != 0)
        printSignatures(temp->nextVirus);
    return list;
}

char bounds(int size){
    char inChar;
    char temp[20];
    fgets(temp, 20, stdin);
    sscanf(temp, "%c", &inChar);
    
    if (inChar <= 48 || inChar > 48 + size){
        printf("%s", "Not within bounds\n");
        exit(0);
    }
    else
        printf("%s", "within bounds\n");
    return inChar; 
}

link* detectViruses(link* list){
    
}














