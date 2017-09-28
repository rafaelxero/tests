/*
 * Calculator
 *
 * gcc -g -c calc.c
 * gcc -g -o add.o calc.o mult.o sub.o
 *
 */

#include "numbers.h"

int main(void) {

  int choice;
  num a, b, c;

  printf("[0] Exit\n[1] Add\n[2] Substract\n[3] Multiply\n");
  scanf("%d", &choice);

  if (!choice)
    return 0;

  printf("Enter 1st number:\n");
  scanf("%d", &a);
  printf("Enter 2nd number:\n");
  scanf("%d", &b);

  switch(choice) {
  case 1:
    c = add(a, b);
    break;
  case 2:
    c = sub(a, b);
    break;
  case 3:
    c = mult(a, b);
    break;
  }

  printf("The answer is: %i\n", c);

  return 0;
}
