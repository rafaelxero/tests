/*
 * gcc -o DifferentPointerType DifferentPointerType.c
 */

#include <stdio.h>

int main(void) {

  double x = 100.1, y;
  int * p;

  // p = &x;
  p = (int *) &x;

  y = *p;

  printf("%f\n", y);

  printf("%p\n", p);
  p++;
  printf("%p\n", p);

  printf("%lu\n", sizeof (int));  // %lu is for long unsigned

  return 0;
}
