/*
 * gcc -o FormalParameters FormalParameters.c
 */

#include <stdio.h>

int is_in(char *s, char c);

int main(void) {
  
  char *s = "Rafael Cisneros";
  char  c = 'M';

  printf("%c is part of the string? %d\n", c, is_in(s, c));

  return 0;
}

/* Return 1 if c is part of string s; 0 otherwise */
int is_in(char *s, char c) {

  while (*s)
    if (*s == c)
      return 1;
    else
      s++;

  return 0;
}
