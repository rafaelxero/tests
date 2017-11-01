/*
 * gcc -o SwitchMenu SwitchMenu.c
 */

#include <stdio.h>

void menu(void);

int main(void) {
  
  menu();

  return 0;
}

void menu(void) {
  
  char ch;

  printf("1. Check Spelling\n");
  printf("2. Correct Spelling Errors\n");
  printf("3. Display Spelling Errors\n");
  printf("Strike Any Other Key to Skip\n");
  printf("      Enter your choice: ");

  ch = getchar();  // read the selection from the keyboard

  printf("ch: %d\n", ch);
  
  switch (ch) {
  case '1':
    printf("Check spelling...\n");
    break;
  case '2':
    printf("Correct errors...\n");
    break;
  case '3':
    printf("Display errors...\n");
    break;
  default:
    printf("No option selected\n");
  }
}
