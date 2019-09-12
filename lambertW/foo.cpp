/*
 * g++ -o foo foo.cpp LambertW.cc
 *
 */

#include "LambertW.h"
#include <iostream>
#include <iomanip>

int main()
{
  std::cout << std::setprecision(10);
  
  double z = 3.14;
  double r = utl::LambertW<0>(z);
  std::cout << "lambert_w0(z) = " << r << std::endl;

  return 0;
}

