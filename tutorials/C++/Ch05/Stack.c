/*
 * gcc -o Stack Stack.c
 */

#include <stdio.h>
#include <stdlib.h>

#define SIZE 5

void push(int i);
int pop(void);

int *tos, *p1, stack[SIZE];

int main(void) {

  int value;
  int i;

  tos = stack;
  p1 = stack;

  do {

    printf("Enter value: ");
    scanf("%d", &value);
    
    if (value != 0 && value != -1)
      push(value);
    else
      printf("value on top is %d\n", pop());

  } while (value != -1);

  printf("values in the stack:\n");

  for (i = 0; i < SIZE; i++)
    printf("%d\n", stack[i]);

  return 0;
}

void push(int i) {

  if (p1 == (tos + SIZE)) {
    printf("Stack Overflow.\n");
    exit(1);
  }

  *p1 = i;

  p1++;
}

int pop(void) {

  if (p1 == tos) {
    printf("Stack Underflow.\n");
    exit(1);
  }

  p1--;

  return *p1;
}
