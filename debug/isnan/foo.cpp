/*
 * g++ -o foo foo.cpp
 * g++ -o foo foo.cpp -std=c++11
 */

#include <iostream>
#include <cmath>

template<typename T>
T saturate(T val, T min, T max) {
    return std::min(std::max(val, min), max);
}

int main(void) {
  
  double y = 10.0;
  double limit = 5.0;

  y = saturate(y, -limit, limit);
  
  if (isnan(y))
    std::cout << "Nan" << std::endl;

  return 0;
}
