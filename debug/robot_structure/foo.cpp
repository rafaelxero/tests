#include <RobotServer/RobotStructureHRG.h>
#include <Model/HumanoidBodyUtil.h>
#include <iostream>

using namespace motion_generator;
using namespace robot_server;

int main(int argc, char *argv[])
{
  HumanoidBodyPtr body(new HumanoidBody());

  /*
  const char *url = "/home/rcisneros/openrtp/share/OpenHRP-3.1/robot/HRP5P/model/HRP5Pmain.wrl";
  
  loadHumanoidBodyFromModelLoader(body, url, argc, argv);
    
  //RobotStructure* robot;
  RobotStructureHRG* robot;
  robot = new RobotStructureHRG(body);
  */
  
  return 0;
}
