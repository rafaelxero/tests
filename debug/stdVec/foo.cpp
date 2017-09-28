/*
 * g++ -o foo foo.cpp -std=c++11 -I /usr/local/include/eigen3 -I /home/rcisneros/src/hrg/hmc2/src/Util/
 */

#include <iostream>
#include <vector>
#include <Eigen/Eigen>
#include <EigenUtil.h>

using namespace hrp;

int main(void) {

  Eigen::Matrix<double, 6, 1> a;
  std::vector<double> b, h, t;

  a << 1, 2, 3, 4, 5, 6;

  b = eigen2std(a);
  h = stdhead(b, 4);
  t = stdtail(b, 4);
  
  for (size_t i = 0; i < b.size(); i++)
    std::cout << b[i] << " ";
  std::cout << std::endl;

  for (size_t i = 0; i < h.size(); i++)
    std::cout << h[i] << " ";
  std::cout << std::endl;

  for (size_t i = 0; i < t.size(); i++)
    std::cout << t[i] << " ";
  std::cout << std::endl;

  
  return 0;
}
