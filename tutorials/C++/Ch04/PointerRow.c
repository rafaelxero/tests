/*
 * gcc -o PointerRow PointerRow.c
 */

#include <stdio.h>

int num[10][10];

void pr_row(int j);

int main(void) {

  num[2][0] = 10;

  pr_row(2);

  return 0;
}

void pr_row(int j) {

  int *p, t;

  // p = (int *) &num[j][0];
  // p = &num[j][0];
  p = num[j];

  for (t = 0; t < 10; ++t)
    printf("%d ", *(p + t));
  printf("\n");
}
