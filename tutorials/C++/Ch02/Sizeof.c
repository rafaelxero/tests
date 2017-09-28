/*
 * gcc -o Sizeof Sizeof.c
 */

#include <stdio.h>

void put_rec(int rec[6], FILE *fp);

int main(void) {

  FILE *fp;
  int rec[6] = {1, 2, 3, 4, 5, 6};

  fp = fopen("text.txt", "w+");

  put_rec(rec, fp);

  return 0;
}

void put_rec(int rec[6], FILE *fp) {

  int len;

  printf("%lu\n", sizeof(char));  // (in bytes)
  printf("%lu\n", sizeof(int));   // (in bytes)

  len = fwrite(rec, sizeof(int)*6, 1, fp);
  
  if (len != 1)
    printf("Write Error\n");
}

/*
 * Output:
 *
 * 1
 * 4
 *
 * hexdump text.txt:
 *
 * 0000000 0001 0000 0002 0000 0003 0000 0004 0000
 * 0000010 0005 0000 0006 0000                    
 * 0000018
 *
 * Explanation:
 * 
 * The first column is the offset in hexadecimal
 * Then each column represents 2 bytes (4 nibbles)
 * Integers appear "byte-reversed" (see Evernote)
 *
 */
