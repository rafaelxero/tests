/*
 * g++ -o foo2 foo2.cpp -std=c++0x `pkg-config --cflags --libs mc_rtc` -I /usr/include/eigen3/ -g
 *
 */

#include <mc_rbdyn/RobotLoader.h>

int main()
{

auto hrp5m = mc_rbdyn::RobotLoader::get_robot_module("HRP5P");

for(const auto & n : hrp5m->ref_joint_order())
{
try
{
    std::cout << n << " " << hrp5m->mb.jointIndexByName(n) << std::endl;
}
catch(...)
{
    std::cout << "No joint " << n << std::endl;
}
}

}
