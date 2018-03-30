#include <mc_rbdyn_urdf/urdf.h>
#include <fstream>

int main()
{
  std::string urdf_path = "/home/rcisneros/src/jrl/catkin_ws/src/hrp5/hrp5_p_description/urdf/hrp5_p_Kumagai.urdf";
  virtualLinks.push_back("base_link");
  //virtualLinks.push_back("rfsensor");
  //virtualLinks.push_back("lfsensor");
  //virtualLinks.push_back("rhsensor");
  //virtualLinks.push_back("lhsensor");
  //virtualLinks.push_back("CAMERA_C");
  //virtualLinks.push_back("CAMERA_X");
  //virtualLinks.push_back("CAMERA_R");
  //virtualLinks.push_back("CAMERA_RH");
  //virtualLinks.push_back("CAMERA_LH");
  //virtualLinks.push_back("gsensor");
  //virtualLinks.push_back("gyrometer");

  virtualLinks.push_back("head_root");
  virtualLinks.push_back("left_camera_optical_frame");
  virtualLinks.push_back("motor");
  virtualLinks.push_back("spindle");
  virtualLinks.push_back("head_hokuyo_frame");
  virtualLinks.push_back("right_camera_optical_frame");

  //headLinks.push_back("Head_Link0");
  //headLinks.push_back("Head_Link1_Plan2");
  
  gripperLinks.push_back("Rthumb_Link0");
  gripperLinks.push_back("Rthumb_Link1");
  gripperLinks.push_back("Rthumb_Link2");
  gripperLinks.push_back("Rindex_Link0");
  gripperLinks.push_back("Rindex_Link1");
  gripperLinks.push_back("Rindex_Link2");
  gripperLinks.push_back("Rmiddle_Link0");
  gripperLinks.push_back("Rmiddle_Link1");
  gripperLinks.push_back("Rmiddle_Link2");

  gripperLinks.push_back("Lthumb_Link0");
  gripperLinks.push_back("Lthumb_Link1");
  gripperLinks.push_back("Lthumb_Link2");
  gripperLinks.push_back("Lindex_Link0");
  gripperLinks.push_back("Lindex_Link1");
  gripperLinks.push_back("Lindex_Link2");
  gripperLinks.push_back("Lmiddle_Link0");
  gripperLinks.push_back("Lmiddle_Link1");
  gripperLinks.push_back("Lmiddle_Link2");
  
  std::vector<std::string> filteredLinks;
  filteredLinks.insert(filteredLinks.end(), virtualLinks.begin(), virtualLinks.end());
  filteredLinks.insert(filteredLinks.end(), headLinks.begin(), headLinks.end());
  filteredLinks.insert(filteredLinks.end(), gripperLinks.begin(), gripperLinks.end());

  if(ifs.is_open())
  {
    std::stringstream urdf;
    urdf << ifs.rdbuf();
    auto res = mc_rbdyn_urdf::rbdyn_from_urdf(urdf.str(), false, filteredLinks);
    std::cout << "MERGE\n";
    mbg.mergeSubBodies(mb.body(0).name(), "WY", {{"HY", {0}}, {"HP", {0}}});
    std::cout << "OK\n";
    mb = mbg.makeMultiBody(mb.body(0).name(), false);
    mbc = rbd::MultiBodyConfig(mb);
    mbc.zero(mb);
    rbd::forwardKinematics(mb, mbc);
    rbd::forwardVelocity(mb, mbc);
  }
}
