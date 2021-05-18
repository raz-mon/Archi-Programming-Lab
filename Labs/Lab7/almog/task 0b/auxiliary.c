#include <stdio.h>
#include <stdlib.h>
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
    void (*fun)(void * L);      // This is the pointer to the function, which gets a char as an argument.
};

void debug_mode(state* s);
void set_file_name(state* s);
void set_unit_size(state* s);
void load2mem(state* s);
void mem_disp(state* s);
void save2file(state* s);
void mem_mod(state* s);
void quit(state* s);

void debug_mode(state* s){
  if(s->debug_mode == 0){
    s->debug_mode = 1;
    printf("Debug flag now on\n");
  }else
  {
    s->debug_mode = 0;
    printf("Debug flag now off\n");
  }
}

void set_file_name(state* s){
  char temp[NAME_LEN];
  fgets(temp, NAME_LEN, stdin);
  sscanf(temp, "%s", s->file_name);
  if(s->debug_mode == 1){
    printf("Debug: file name set to: %s\n" , s->file_name);
  }
}

void set_unit_size(state* s){
  printf("Please enter wanted size: ");
  char c = fgetc(stdin) - 48;
  char temp = fgetc(stdin);
  if(c == 1 | c == 2 | c == 4){
    s->unit_size = (int)c;
    if(s->debug_mode == 1){
      printf("Debug: set size to %d\n" , s->unit_size);
    }
  }else{
    fprintf(stderr , "Error: size is not valid\n");
  }
}

void load2mem(state* s){
  printf("not implemented yet\n");
}

void mem_disp(state* s){
  printf("not implemented yet\n");
}

void save2file(state* s){
  printf("not implemented yet\n");
}

void mem_mod(state* s){
  printf("not implemented yet\n");
}

void quit(state* s){
    if(s->debug_mode == 1){
      printf("quitting\n");
    }
    free(s);
    exit(0);
}