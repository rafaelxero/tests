/*
 * gcc -o Putstr Putstr.c
 */

#include <stdio.h>

void putstr1(char *s);
void putstr2(char *s);

int main(void) {

  char s[11] = "Hola Alfa\n";

  putstr1(s);
  putstr2(s);

  return 0;
}

void putstr1(char *s) {

  register int t;

  for (t = 0; s[t]; ++t)
    putchar(s[t]);
}

void putstr2(char *s) {

  while (*s)
    putchar(*s++);  // putchar (*(s++))
}
