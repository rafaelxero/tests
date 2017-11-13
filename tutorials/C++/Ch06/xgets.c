/*
 * gcc -o xgets xgets.c
 */

#include <stdio.h>

char* xgets(char *s);

int main(void) {

  char s[80];
  
  xgets(s);

  printf("%s\n", s);

  return 0;
}

char* xgets(char *s) {

  char ch, *p;
  int t;

  p = s;

  for (t = 0; t < 80; ++t) {
  
    ch = getchar();

    switch (ch) {
    
    case '\n':
      s[t] = '\0';
      return p;

    case '\b':
      if (t > 0) t--;
      break;

    default:
      s[t] = ch;
    }
  }

  s[79] = '\0';
  return p;
}
