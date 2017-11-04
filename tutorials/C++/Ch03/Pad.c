/*
 * gcc -o Pad Pad.c
 */

#include <stdio.h>
#include <string.h>

void pad(char *s, int length);

int main(void) {

  char str[80];
  
  strcpy(str, "this is a test");
  pad(str, 40);
  printf("%s\n", str);
  printf("%d\n", strlen(str));
  
  return 0;
}

void pad(char *s, int length) {

  int l;
  l = strlen(s);
  
  while(l < length) {
    s[l] = ' ';
    l++;
  }
  s[l] = '\0';
}
