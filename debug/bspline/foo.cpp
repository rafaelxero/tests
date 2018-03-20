/*
 * g++ -o foo foo.cpp /home/rcisneros/src/hrg/hmc2/src/TrajectoryGenerator/BSplineInterpolator.cpp
 *     -I /home/rcisneros/src/hrg/hmc2/src/ -I /usr/include/eigen3/ -std=c++11 -O2 -g -DNDEBUG
 *
 * See compile.sh
 *
 */

#include <TrajectoryGenerator/BSplineInterpolator.h>
#include <iostream>

using namespace interpolator;

int main()
{
  BSplineInterpolator* m_interpolator = new BSplineInterpolator(0.005);
  
  Trajectory m_res;

  Eigen::VectorXd v0(3);
  std::vector<Eigen::VectorXd> sk(3);
  Eigen::VectorXd vf(3);
  double T = 1;

  v0 << 0.0, 0.5, 0.0;

  sk[0].resize(3);
  sk[0] << 0.0, 0.0, 0.8;

  sk[1].resize(3);
  sk[1] << 0.1, 0.0, 0.5;

  sk[2].resize(3);
  sk[2] << 0.0, 0.0, 0.7;

  vf << 0.0, 0.0, 0.0;
  
  m_interpolator->update(v0, sk, vf, T);
  
  /*
  std::vector<Eigen::VectorXd> sk_cal = m_interpolator->calculate_sk();
  
  for (int i = 0; i < sk.size(); i++)
    std::cout << "sk_cal = " << sk_cal[i].transpose() << std::endl;
  std::cout << std::endl;
  */

  for (int i = 0; i <= 1000; i+=5) {
    double t = static_cast<double>(i) / 1000.0;
    m_res = m_interpolator->interpolate(t);
    std::cout << "m_res.s(" << t << ") = " << m_res.st.transpose() << std::endl;
  }
  
  return 0;
}
