/*
 * gcc -o PointerOperators PointerOperators.c
 */

#include <stdio.h>

int main(void) {

  int target, source;
  int *m;

  source = 10;
  m = &source;
  target = *m;
  
  printf("Pointer: %p\n", m);
  printf("Value:   %d\n", target);

  return 0;
}
