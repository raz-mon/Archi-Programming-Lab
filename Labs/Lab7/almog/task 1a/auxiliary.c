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

int strToloc();
int strToint();

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
  if(strcmp(s->file_name, "") == 0){
    fprintf(stderr, "Error: invalid file name\n");
  }else{
    FILE * file = fopen(s->file_name, "r");
    if(file == NULL){
      perror("could not open file\n");
    }else{
      printf("\nPlease enter <location> <length>\n");
      int loc = strToloc();
      if(loc == -1){ return; }
      int length = strToint();

      if(s->debug_mode == 1){
          printf("========================================================\n\n");
          printf("File name: %s   |   File location: %d   |   File length: %d\n\n",
          s->file_name, loc, length);
          printf("========================================================\n\n");
      }
      size_t offset = fseek(file,loc,SEEK_SET);
      size_t cop = fread(s->mem_buf, 1, length * s->unit_size, file);
      fclose(file);
      printf("Loaded %d units into memory\n", cop);
      printf("mem buffer: %s\n", s->mem_buf);
    }
  }
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




int strToloc(){
  char c;
  char loc[100];
  int counter = 0;
    while((c=fgetc(stdin))!=32){
      if(c > 47 && c < 58){
        loc[counter++] = c - 48;
      }else if(c > 96 && c < 103){
        loc[counter++] = c - 87;
      }else if(c > 64 && c < 71){
        loc[counter++] = c - 55;
      }else{
        fprintf(stderr, "Error: wrong input\n");
        return -1;
      }
    }

    int final_loc = 0;
    for(int i = 0; i < counter; i++){
      int power = 1;
      for(int j = 0; j < counter - i - 1; j++){
        power = power * 16;
      }
      final_loc += loc[i] * power;
    }
    return final_loc;
}

int strToint(){
    char temp[100];
    char str[100];
    fgets(temp,100,stdin);
    sscanf(temp,"%s",str);
    return atoi(str);
}