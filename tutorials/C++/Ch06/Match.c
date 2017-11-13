/*
 * gcc -o Match Match.c
 */

#include <stdio.h>

char* match(char c, char *s);

int main(void) {

  char s[80], *p, ch;

  gets(s);
  ch = getchar();
  p = match(ch, s);

  if (*p)
    printf("%s\n", p);
  else
    printf("No match found\n");

  return 0;
}

char* match(char c, char*s) {

  while (c != *s && *s) s++;
  return(s);
}
