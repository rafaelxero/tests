/*
 * gcc -o MyPutw MyPutw.c
 */

#include <stdio.h>

union pw {
  short int i;
  char ch[2];
};

int my_putw(short int num, FILE *fp);

int main(void) {

  FILE *fp;

  fp = fopen("test.tmp", "wb+");
  my_putw(1000, fp);
  fclose(fp);

  return 0;
}

int my_putw(short int num, FILE *fp) {

  union pw word;
  
  word.i = num;

  putc(word.ch[0], fp);
  return putc(word.ch[1], fp);
}
