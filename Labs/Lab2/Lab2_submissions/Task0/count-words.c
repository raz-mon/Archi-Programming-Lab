/* $Id: count-words.c 858 2010-02-21 10:26:22Z tolpin $ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* words(int count){
  char* words = (char *)malloc(sizeof(char) * 5);
  *(words) = 'w';
  *(words + 1) = 'o';
  *(words + 2) = 'r';
  *(words + 3) = 'd';
  *(words + 4) = 's';
  *(words + 5) = '\0';  
  if (count == 1)
    *(words + 4) = '\0';
  return words;
}



/* return string "word" if the count is 1 or "words" otherwise */

/*    old func:
char *words(int count) {
  char *words = "words";
  if(count==1)
    words[strlen(words)-1] = '\0';
  return words;
}
*/

/* print a message reportint the number of words */
int print_word_count(char **argv) {
  int count = 0;
  char **a = argv;
  while(*(a++))
    ++count;
  char* words_ = words(count);  
  printf("The sentence contains %d %s.\n", count, words_);
  free(words_);
  return count;
}

/* print the number of words in the command line and return the number as the exit code */
int main(int argc, char **argv) {
  return print_word_count(argv+1);
}

   
