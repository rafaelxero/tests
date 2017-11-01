/*
 * g++ -o foo foo.cpp
 *
 * https://stackoverflow.com/questions/9705660/check-glibc-version-for-a-particular-gcc-compiler
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <gnu/libc-version.h>

int main(int argc, char *argv[]) {
  printf("GNU libc version: %s\n", gnu_get_libc_version());
  exit(EXIT_SUCCESS);
}
