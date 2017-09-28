/*
 * gcc -o StaticLocalVariables StaticLocalVariables.c
 */

#include <stdio.h>

int series(void);

int main(void) {

  printf("%d\n", series());
  printf("%d\n", series());
  printf("%d\n", series());

  return 0;
}

int series(void) {

  static int series_num = 100;

  series_num = series_num+23;
  return series_num;
}
