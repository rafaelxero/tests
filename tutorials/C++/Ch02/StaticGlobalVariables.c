/*
 * gcc -o StaticGlobalVariables StaticGlobalVariables.c
 */

#include <stdio.h>

static int series_num;
void series_start(int seed);
int series(void);

int main(void) {
  
  series_start(50);
  printf("%d\n", series());
  printf("%d\n", series());
  printf("%d\n", series());
}

void series_start(int seed) {

  series_num = seed;
}

int series(void) {

  series_num += 23;
  return series_num;
}
