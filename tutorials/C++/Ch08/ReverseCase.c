/*
 * gcc -o ReverseCase ReverseCase.c
 */

#include <stdio.h>
#include <ctype.h>
// #include <conio.h>

int main(void) {

  char ch;
  
  printf("Enter some text (type a period to quit).\n");
  
  do {

    ch = getchar();
    // ch = getch();

    if (islower(ch))
      ch = toupper(ch);
    else
      ch = tolower(ch);

    putchar(ch);

  } while (ch != '.');

  puts("The end.");

  return 0;
}
