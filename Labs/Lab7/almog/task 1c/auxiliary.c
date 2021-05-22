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
void read_units_to_memory(FILE* input, char* buffer, int count, int unit_size);
void print_units(FILE* output, char* buffer, int count, int unit_size);
char* unit_to_format(int unit);

void write_units(FILE* output, char* buffer, int count, size_t unit_size);

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

      unsigned int loc;
      int length;
      char str[100];
      fgets(str,100,stdin);
      sscanf(str,"%x %d", &loc, &length);

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
  printf("Enter address and length\n");
  int u;
  unsigned int addr;
  char str[100];
  fgets(str,100,stdin);
  sscanf(str,"%x %d", &addr, &u);

  // Debug option
  if (s->debug_mode == 1)
      printf("Debug: displaying memory at location %x, of length %d\n", addr, u);

  if(addr == 0){
    print_units(stdout, s->mem_buf, u, s->unit_size);
  }else{
    FILE * file = fopen(s->file_name, "r");
    char buffer[u * s->unit_size];

    size_t offset = fseek(file,addr,SEEK_SET);
    size_t readed = fread(buffer,s->unit_size,u,file);
    print_units(stdout, buffer, u, s->unit_size);
    fclose(file);
  }
  
}

void save2file(state* s){
  printf("please enter <source-address> <target-location> <length>\n");
  char inLine[200];
  fgets(inLine, 201, stdin);
  int source_add, tar_loc, len;
  sscanf(inLine, "%x %x %d", &source_add, &tar_loc, &len);
  // Debug option
  if (s->debug_mode == 1)
      printf("copying %d units from source location: %x, to target location: %x\n", len, source_add, tar_loc);

  FILE* file = fopen(s->file_name, "r+");
  if (file == NULL){
      perror("could not open file: ");
      exit(0);
  }else{
    fseek(file, 0, SEEK_END);
    int file_len = ftell(file);
    fseek(file, 0, SEEK_SET);
    if(file_len < tar_loc){
      fprintf(stderr, "Error: target location is invalid\n");
      fclose(file); exit(0);
    }
  }
  //fseek(file, 0, SEEK_SET);
  fseek(file, tar_loc, SEEK_SET);
  write_units(file, s->mem_buf + source_add, len, s->unit_size);
  fclose(file);
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

/* Reads units from file */
void read_units_to_memory(FILE* input, char* buffer, int count, int unit_size) {
    fread(buffer, unit_size, count, input);  
}

/* Prints the buffer to screen by converting it to text with printf */
void print_units(FILE* output, char* buffer, int count, int unit_size) {
    char* end = buffer + unit_size*count;
    char* bufferhex = buffer;
    fprintf(output, "Hexadecimal\n===========\n");
    while (bufferhex < end) {
        //print hexa
        int var = *((unsigned int*)(bufferhex));
        fprintf(output, unit_to_format(unit_size), var);
        bufferhex += unit_size;
    }

    fprintf(output, "\nDecimal\n=======\n");
    while (buffer < end) {
        //print ints
        int var = *((int*)(buffer));
        fprintf(output, "%d", var);
        fprintf(output, "\n");
        buffer += unit_size;
    }
}

char* unit_to_format(int unit) {
    static char* formats[] = {"%hhX\n", "%hX\n", "No such unit", "%X\n"};
    return formats[unit-1];
    /* If the above is too confusing, this can also work:
    switch (unit_size) {
        case 1:
            return "%#hhx\n";
        case 2:
            return "%#hx\n";
        case 4:
            return "%#hhx\n";
        default:
            return "Unknown unit";
    }
    */
}

void write_units(FILE* output, char* buffer, int count, size_t unit_size) {
    fwrite(buffer, unit_size, count, output);
}
