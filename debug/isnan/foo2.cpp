/*
 * g++ -o foo2 foo2.cpp
 * g++ -o foo2 foo2.cpp -std=c++11
 */

#include <iostream>
#include <cmath>

#if __cplusplus >= 201103L
#if __GLIBC_PREREQ(2,23)
#warning GLIBC version 2.23 or greater
using std::isnan;
#else
#warning GLIBC version previous to 2.23
#define isnan std::isnan
#endif
#endif

int main(void) {

  double y = 0;
  if (isnan(y))
    std::cout << "Nan" << std::endl;
  else
    std::cout << "Not Nan" << std::endl;
  return 0;
}
