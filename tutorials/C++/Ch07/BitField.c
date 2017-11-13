/*
 * gcc -o BitField BitField.c
 */

#include <stdio.h>

struct status_type {
  unsigned : 4;
  unsigned delta_cts: 1;
  unsigned delta_dsr: 1;
};

int main(void) {
  
  struct status_type status;

  printf("%lu\n", sizeof status);
  
  return 0;
}
