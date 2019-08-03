/*
 * g++ -o foo foo.cpp
 */

#include <stdio.h>
#include <iostream>

class TransListenerImpl
{
public: 
  TransListenerImpl(double dt, int cnt_max = 2) :
    m_cnt_max(cnt_max),
    m_count(m_cnt_max),
    m_dt(dt)
  {
    std::cout << "m_cnt_max = " << m_cnt_max << std::endl;
    std::cout << "m_count = " << m_count << std::endl;
    std::cout << "m_dt = " << m_dt << std::endl;
  }

private:
  int m_cnt_max;
  int m_count;
  double m_dt;
};

int main(int argc, char *argv[])
{
  float period = 0.001;
  float ref_period = 0.005;

  std::cout << "ref_period / period = ";
  printf("%11.61f\n", ref_period / period);
  std::cout << "(int) (ref_period / period) = " << (int) (ref_period / period) << std::endl;

  std::cout << "(ref_period * 1000) / (period * 1000) = ";
  printf("%11.61f\n", (ref_period * 1000) / (period * 1000));
  std::cout << "(int) ((ref_period * 1000) / (period * 1000)) = " << (int) ((ref_period * 1000) / (period * 1000)) << std::endl;
  
  TransListenerImpl tl(period, (int) ((ref_period * 1000) / (period * 1000)));

  return 0;
}
