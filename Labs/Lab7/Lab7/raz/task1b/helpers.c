#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h> // Prototype for basename() function
#include <sys/types.h>
#include <unistd.h>

#define  NAME_LEN  128
#define  BUF_SZ    10000

#define KB(i) ((i)*1<<10)
#define INT sizeof(int)
#define SHORT sizeof(short)
#define BYTE sizeof(char)

char* unit_to_format(int unit);
void read_units_to_memory(FILE* input, char* buffer, int count);
void print_units(FILE* output, char* buffer, int count);
void print_units_dec(FILE* output, char* buffer, int count);
void write_units(FILE* output, char* buffer, int count);

typedef struct {
  char debug_mode;
  char file_name[NAME_LEN];
  int unit_size;
  unsigned char mem_buf[BUF_SZ];
  size_t mem_count;
  /*
   .
   .
   Any additional fields you deem necessary. If you make a change here, change in main too.
  */
} state;

state* s;

struct fun_desc {       // Definition of the struct.
        char* name;
        void (*fun)(state);      // This is the pointer to the function, which gets a state as an argument.
    };


void TogDMode(state* s){
    if(s->debug_mode == 0){
        printf("Debug flag now on\n");
        s->debug_mode = 1;
    }
    else{
        printf("Debug flag now off\n");
        s->debug_mode = 0;
    }
}

void SetFileName(state* s){
    printf("enter file name: ");
    char newName_temp[128];
    fgets(newName_temp, 129, stdin);
    //char newName[128];
    sscanf(newName_temp, "%s", s->file_name);
    printf("\n");
    if (s->debug_mode == 1)
        printf("Debug: file name set to %s\n", s->file_name);
}

void SetUnitSize(state* s){
    char inSize_temp[3]; 
    char inSize;
    fgets(inSize_temp, 3, stdin);
    sscanf(inSize_temp, "%c", &inSize);
    if (inSize == '1' | inSize == '2' | inSize == '4'){
        s->unit_size = inSize - 48;
        if (s->debug_mode == 1)
            printf("Debug: set size to %d\n", s->unit_size);
    }
    else
        fprintf(stderr, "size entered not valid sir.\n");
}

void LoadIntoMem(state* s){
    if (strcmp(s->file_name, "") == 0){
        fprintf(stderr, "No file name loaded!!\n");
        return;
    }
    FILE* file = fopen(s->file_name, "r");
    if (file == NULL){
        perror("Could not open file: ");
        return;
    }
    // File opened fine.
    printf("Please enter <location> <length>\n");  // Notice that location is in hexa and length in decimal!
    char input[100];
    fgets(input, 101, stdin);
    int len;
    int loc;
    sscanf(input, "%x %d", &loc, &len);
    // sscanf( dtm, "%s %s %d  %d", weekday, month, &day, &year );
    if (s->debug_mode == 1){
        printf("file name: %s\nlocation: %0x\nlength: %d\n", s->file_name, loc, len);
    }
    fseek(file, loc, SEEK_SET);
    read_units_to_memory(file, s->mem_buf, len);    
    fclose(file);
    s->mem_count += len;
}

void memDisp(state* s){
    printf("Enter address and length\n");
    char input[100];
    fgets(input, 101, stdin);
    int add;
    int len;
    sscanf(input, "%x %d", &add, &len);

    // Debug option
    if (s->debug_mode == 1)
        printf("Debug: displaying memory at location %x, of length %d\n", add, len);

    if (add == 0){
        printf("Hexadecimal\n===========\n");
        print_units(stdout, s->mem_buf, len);

        printf("Decimal\n===========\n\n");
        print_units_dec(stdout, s->mem_buf, len);
    }
    else{
        FILE* file = fopen(s->file_name, "r");
        if (file == NULL){
            perror("Could not open file: ");
            return;
        }
        fseek(file, add, SEEK_SET);
        char fromFile[BUF_SZ];
        read_units_to_memory(file, fromFile, len);

        printf("Hexadecimal\n===========\n");
        print_units(stdout, fromFile, len);

        printf("Decimal\n===========\n\n");
        print_units_dec(stdout, fromFile, len);

        fclose(file);
    }
}

void saveToFile(state* s){
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
    }
    //fseek(file, 0, SEEK_SET);
    fseek(file, tar_loc, SEEK_SET);
    write_units(file, s->mem_buf + source_add, len);
    fclose(file);
}

void modifyMem(state* s){
    printf("yet to be implemented");
}

void quit(state* s){
    printf("Quitting.. it was nice while it lasted.\n");
    exit(0);
}






char* unit_to_format(int unit) {
    static char* formats[] = {"%#hhx\n", "%#hx\n", "No such unit", "%#x\n"};
    return formats[s->unit_size-1];
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


/************ These are the functions that actually do the work ***************/
/* Reads units from file */
void read_units_to_memory(FILE* input, char* buffer, int count) {
    fread(buffer, s->unit_size, count, input);
}

/* Prints the buffer to screen by converting it to text with printf */
void print_units(FILE* output, char* buffer, int count) {
    char* end = buffer + s->unit_size*count;
    while (buffer < end) {
        //print ints
        int var = *((int*)(buffer));
        fprintf(output, unit_to_format(s->unit_size), var);
        buffer += s->unit_size;
    }
}

/* Prints the buffer to screen by converting it to text with printf, the decimal version. */
void print_units_dec(FILE* output, char* buffer, int count) {
    char* end = buffer + s->unit_size*count;
    while (buffer < end) {
        //print ints
        int var = *((int*)(buffer));
        fprintf(output, "%d\n", var);
        buffer += s->unit_size;
    }
}

/* Writes buffer to file without converting it to text with write */
void write_units(FILE* output, char* buffer, int count) {
    fwrite(buffer, s->unit_size, count, output);
}
/*****************************************************************************/
