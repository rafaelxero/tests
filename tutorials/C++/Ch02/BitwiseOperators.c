/*
 * gcc -o BitwiseOperators BitwiseOperators.c
 */

// A bit shift example

#include <stdio.h>

int main(void) {

  unsigned int i;
  int j;
  
  i = 1;

  for (j = 0; j < 4; j++) {
    i = i << 1;
    printf("Left  shift %d: %d\n", j + 1, i);
  }

  for (j = 0; j < 4; j++) {
    i = i >> 1;
    printf("Right shift %d: %d\n", j + 1, i);
  }

  return 0;
}
