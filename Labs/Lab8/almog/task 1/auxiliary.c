#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <fcntl.h>
#include <sys/mman.h>
       //#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#define  NAME_LEN  128
#define  BUF_SZ    10000

typedef struct {
    char debug_mode;
    char file_name[NAME_LEN];
    int unit_size;
    unsigned char mem_buf[BUF_SZ];
    size_t mem_count;

    //Lab 8
    int Currentfd;
    void* map_start;
    struct stat fd_stat;
    Elf32_Ehdr * header;
    int num_of_section_headers;
    

} state;


struct fun_desc {       // Definition of the struct.
    char* name;
    void (*fun)(void * L);      // This is the pointer to the function, which gets a char as an argument.
};

void debug_mode(state* s);
void exm_elf_file(state* s);
void print_sec_names(state* s);
void print_sym(state* s);
void rel_tab(state* s);
void quit(state* s);

void beforExam(state * s);
void afterExam(state * s);
void printShtab(state * s);

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

void exm_elf_file(state* s){
  printf("Enter ELF file name to be examine\n");
  char str[100];
  fgets(str,100,stdin);
  sscanf(str,"%s",s->file_name);

  beforExam(s);
  
  printf("ELF Header:\n");
  printf("  %-8s:%-3X%-3X%-3X%-3X\n" ,"Magic",s->header->e_ident[EI_MAG0], s->header->e_ident[EI_MAG1], s->header->e_ident[EI_MAG2], s->header->e_ident[EI_MAG3]);
  switch (s->header->e_ident[EI_DATA])
  {
  case ELFDATANONE:
    printf("  %-35s%s\n" , "Data:","ELFDATANONE");
    break;
  case ELFDATA2LSB:
    printf("  %-35s%s\n" , "Data:","ELFDATA2LSB");
    break;
  case ELFDATA2MSB:
    printf("  %-35s%s\n" , "Data:","ELFDATA2MSB");
    break;
  }
  
  printf("  %-35s%X\n" , "Entry point:", s->header->e_entry);
  printf("  %-35s%d bytes\n" , "Start of section header:", s->header->e_shoff);
  printf("  %-35s%d bytes\n" , "Number of section headers:" , s->header->e_shnum);
  printf("  %-35s%d bytes\n" , "Start of program header:" , s->header->e_phoff);
  printf("  %-35s%d bytes\n" , "Number of program headers:" , s->header->e_phnum);
  printf("  %-35s%d bytes\n" , "Size of section headers:" , s->header->e_shentsize);
  printf("  %-35s%d bytes\n" , "Size of program headers:" , s->header->e_phentsize);

  if(s->debug_mode == 1){
    printf("  %-35s%d bytes\n" , "Section header string table index:" , s->header->e_shstrndx);
  }

  printShtab(s);
  afterExam(s);
}


void print_sec_names(state* s){
  printf("not implemented yet\n");
}

void print_sym(state* s){
  printf("not implemented yet\n");
}

void rel_tab(state* s){
  printf("not implemented yet\n");
}

void quit(state* s){
    if(s->debug_mode == 1){
      printf("quitting\n");
    }
    free(s);
    exit(0);
}

void beforExam(state * s){
  if(s->Currentfd != -1){
    if( (close(s->Currentfd)) != 0 ) {
      perror("error in close");
      exit(-1);
    }
  }
  if( (s->Currentfd = open(s->file_name, O_RDWR)) < 0 ) {
    perror("error in open");
    exit(-1);
  }

  if( fstat(s->Currentfd, &s->fd_stat) != 0 ) {
    perror("stat failed");
    exit(-1);
  }

  if ( (s->map_start = mmap(0, s->fd_stat.st_size, PROT_READ | PROT_WRITE , MAP_SHARED, s->Currentfd, 0)) == MAP_FAILED ) {
    perror("mmap failed");
    exit(-4);
  }
  s->header = (Elf32_Ehdr *) s->map_start;
}
void afterExam(state * s){
  if(munmap(s->map_start, s->fd_stat.st_size) < 0){
    perror("munmap failed");
    exit(-4);
  }

  if( (s->Currentfd = close(s->Currentfd)) < 0 ) {
    perror("error in close");
    exit(-1);
  }

  s->Currentfd = -1;
}
void printShtab(state * s){
  Elf32_Shdr * sheader = (Elf32_Shdr *)(s->map_start + s->header->e_shoff);

  Elf32_Shdr * shstr_tab = (Elf32_Shdr *)(s->map_start + s->header->e_shoff) + s->header->e_shnum - 3;
  Elf32_Off shstr_off = shstr_tab->sh_offset;

  printf("\nSection Headers:\n");
  printf("  %-7s%-20s%-13s%-9s%-8s%-8s\n", "[Nr]", "Name","Type","Addr","Off","Size");

  for(int i = 0; i < s->header->e_shnum; i++){
    char * sh_name = (s->map_start + shstr_off + sheader->sh_name);
    printf("  [%2d%-4c%-20s%-13X%-9X%-8X%-8X\n",i ,']', sh_name, sheader->sh_type,sheader->sh_addr,
    sheader->sh_offset,sheader->sh_size);
    sheader += 1;
  }
}
