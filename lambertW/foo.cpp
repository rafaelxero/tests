/*
 * g++ -o foo foo.cpp LambertW.cc
 *
 */

#include "LambertW.h"
#include <iostream>
#include <iomanip>
#include <time.h>

int main()
{
  std::cout << std::setprecision(10);
  
  double z = 3.14;

  clock_t time = clock();
  double r = utl::LambertW<0>(z);
  time = (int) (clock() - time);

  std::cout << "lambert_w0(z) = " << r << std::endl;
  std::cout << "Computed in " << time << std::endl;
  std::cout << "CLOCKS_PER_SEC = " << CLOCKS_PER_SEC << std::endl;
  
  return 0;
}

