/*
 * gcc -o PointerFunction1 PointerFunction1.c
 */

#include <stdio.h>
#include <string.h>

void check(char *a, char *b, int (*cmp)(const char *, const char *));

int main(void) {

  char s1[80], s2[80];
  int (*p)(const char *, const char *);

  p = strcmp;

  gets(s1);
  gets(s2);

  check(s1, s2, p);
  // check(s1, s2, strcmp);

  return 0;
}

void check(char *a, char *b, int (*cmp)(const char *, const char *)) {

  printf("Testing for equality.\n");
  
  // if (!cmp(a, b))
  if (!(*cmp)(a, b))
    printf("Equal\n");
  else
    printf("Not Equal\n");
}
