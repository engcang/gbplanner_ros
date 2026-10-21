#include <cstdlib>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <ros/ros.h>

#include "gbplanner/gbplanner.h"

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  google::ParseCommandLineFlags(&argc, &argv, false);

  ros::init(argc, argv, "gbplanner_node");
  ros::NodeHandle nh;
  ros::NodeHandle nh_private("~");

  //Codex: Cover the remaining rand()-based global graph choices as well as the C++ samplers.
  int evaluation_seed;
  if (ros::param::get("/evaluation_seed", evaluation_seed))
    std::srand(static_cast<unsigned int>(evaluation_seed));

  explorer::Gbplanner planner(nh, nh_private);

  ros::spin();

  return 0;
}
