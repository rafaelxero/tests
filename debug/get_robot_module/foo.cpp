/*
 * g++ -o foo foo.cpp /home/rcisneros/src/jrl/mc_rtc/src/mc_rbdyn/RobotLoader.cpp /home/rcisneros/openrtp/lib/libmc_rtc_loader.so.1.0.0 -std=c++0x `pkg-config --cflags --libs mc_rtc` -I/usr/include/eigen3/ -g
 *
 */

#include <mc_rbdyn/RobotLoader.h>
#include <fstream>

int main()
{
  // Previous tests by Pierre
  /*
    std::string path = "/home/rcisneros/src/jrl/catkin_ws/src/hrp5/hrp5_p_description/urdf/hrp5_p_Kumagai.urdf";
    std::ifstream ifs(path);
    std::stringstream urdf;
    urdf << ifs.rdbuf();
    auto res = mc_rbdyn_urdf::rbdyn_from_urdf(urdf.str(), false, {});
    rbd::MultiBody mb = res.mb;
    rbd::MultiBodyConfig mbc = res.mbc;
  */

  mc_rbdyn::RobotLoader::has_robot("HRP5P");
  //std::shared_ptr<mc_rbdyn::RobotModule> = mc_rbdyn::RobotLoader::get_robot_module("HRP5P");
  
  return 0;
}
