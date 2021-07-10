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
   Any additional fields you deem necessary
  */
} state;

struct fun_desc {       // Definition of the struct.
        char* name;
        void (*fun)(state);      // This is the pointer to the function, which gets a state as an argument.
    };


void TogDMode(state* s){
    if(s->debug_mode == 0){
        printf("Debug flag now on");
        s->debug_mode = 1;
    }
    else{
        printf("Debug flag now off");
        s->debug_mode = 0;
    }
}

void SetFileName(state* s){
    char* newName_temp;
    fgets(newName_temp, 128, stdin);
    // char newName[128];
    sscanf(newName_temp, "%s", s->file_name);
    if (s->debug_mode == 1)
        printf("Debug: file name set to %s", s->file_name);
}

void SetUnitSize(state* s){
    char inSize = fgetc(stdin);
    if (inSize == 1 | inSize == 2 | inSize == 4){
        s->unit_size == inSize;
        if (s->debug_mode == 1)
            printf("Debug: set size to %d", s->unit_size);
    }
    else
        fprintf(stderr, "size entered not valid sir.");
}

void LoadIntoMem(state* s){
    printf("yet to be implemented");
}

void memDisp(state* s){
    printf("yet to be implemented");
}

void modifyMem(state* s){
    printf("yet to be implemented");
}

void saveToFile(state* s){
    printf("yet to be implemented");
}

void quit(state* s){
    printf("quitting..\n");
    exit(0);
}