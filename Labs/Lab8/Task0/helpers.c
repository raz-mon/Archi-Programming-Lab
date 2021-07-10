#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h> // Prototype for basename() function
#include <sys/types.h>
#include <unistd.h>
#include <elf.h>
#include <fcntl.h>
#include <sys/mman.h>

#define NAME_LEN 128

typedef struct {
    char debug_mode;
    char file_name[NAME_LEN];
    int Currentfd;
    int st_size;
    void* map_start;
    struct stat fd_stat;
    Elf32_Ehdr * header;
    int num_of_section_headers;
} state;

state* s;

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

void quit(state* s){
    printf("Quitting.. it was nice while it lasted.\n");
    if (s->Currentfd != -1)
        close(s->Currentfd);
    // printf();       // Bytes 1,2,3 of the magic number.
    // printf();       // Entry point.
    exit(0);
}

void examine_ELF_FILE(state* s){
  printf("Enter ELF file name to be examine\n");
  char str[100];
  fgets(str,100,stdin);
  sscanf(str,"%s",s->file_name);

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
  printf("  %-35s%d (bytes)\n" , "Start of section header:", s->header->e_shoff);
  printf("  %-35s%d (bytes)\n" , "Number of section headers:" , s->header->e_shnum);
  printf("  %-35s%d (bytes)\n" , "Size of each entry in section headers:" , s->header->e_shentsize);
  printf("  %-35s%d (bytes)\n" , "Start of program header:" , s->header->e_phoff);
  printf("  %-35s%d (bytes)\n" , "Number of program headers:" , s->header->e_phnum);
  printf("  %-35s%d (bytes)\n" , "Size of each entry in program headers:" , s->header->e_phentsize);
  Elf32_Shdr * sheader = (Elf32_Shdr *)(s->map_start + s->header->e_shoff);
  Elf32_Phdr * pheader = (Elf32_Phdr *)s->header->e_phoff;

  printf("\nSection Headers:\n");
  printf("  %-7s%-10s%s\n", "[Nr]", "Name","Size");
  //printf("\n\n check %d \n\n" , sheader->sh_size);

  printf("%d\n", sheader->sh_size);
  sheader += s->header->e_shentsize;

  printf("%d\n", sheader->sh_size);
  sheader += s->header->e_shentsize;

  printf("%d\n", sheader->sh_size);
  sheader += s->header->e_shentsize;
  /*
  for(int i = 0; i < s->header->e_shnum; i++){
    //printf("  [%-7d]%-10d%d\n" ,i , sheader->sh_name, sheader->sh_size);
    printf("  [%2d%-4c%-10d%d\n",i ,']', sheader->sh_name, sheader->sh_size);
    sheader += s->header->e_shentsize;
    //sheader = s->map_start + s->header->e_shoff + sheader->sh_offset + sheader->sh_size;
    //sheader = s->map_start + sizeof(sheader) + sheader->sh_offset;
  }
*/
}
void print_section_names(state* s){

}

void print_symbols(state* s){

}

void relocation_tables(state* s){

}


