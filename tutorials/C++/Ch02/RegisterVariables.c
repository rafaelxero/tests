/*
 * gcc -o RegisterVariables RegisterVariables.c
 */
 
#include <stdio.h>
#include <time.h>

int main(void) {

  clock_t start, end;
  double cpu_time_used;

  int res;

  start = clock();
  res = int_pwr(2, 300);
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

  printf("Without register: %f\n", cpu_time_used);

  start = clock();
  res = int_pwr_reg(2, 300);
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

  printf("Using   register: %f\n", cpu_time_used);

}

int int_pwr(int m, int e) {

  int temp;

  temp = 1;

  for(; e; e--)
    temp = temp * m;

  return temp;
}

int int_pwr_reg(register int m, register int e) {

  register int temp;

  temp = 1;

  for(; e; e--)
    temp = temp * m;

  return temp;
}

/*
 * Output:
 *
 * Without register: 0.000006
 * Using   register: 0.000003
 *
 */
