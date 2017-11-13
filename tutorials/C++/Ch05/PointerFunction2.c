/*
 * gcc -o PointerFunction2 PointerFunction2.c
 */

#include <stdio.h>
#include <string.h>

void check(char *a, char *b, int (*cmp)(const char *, const char *));

int numcmp(const char *a, const char *b);


int main(void) {

  char s1[80], s2[80];

  gets(s1);
  gets(s2);

  if (isalpha(*s1))
    check(s1, s2, strcmp);
  else
    check(s1, s2, numcmp);

  return 0;
}

void check(char *a, char *b, int (*cmp)(const char *, const char *)) {

  printf("Testing for equality.\n");
  
  if (!(*cmp)(a, b))
    printf("Equal\n");
  else
    printf("Not Equal\n");
}

int numcmp(const char *a, const char *b) {

  printf("Entered to numcmp...\n");

  if (atoi(a) == atoi(b))
    return 0;
  else
    return 1;
}
