#pragma once

#include <chrono>

#include <std_msgs/Float64MultiArray.h>

//Codex: Instrument the planner's actual map callback without maintaining a second evaluation map.
template <class BaseServer>
class TimedVoxbloxServer : public BaseServer {
 public:
  using BaseServer::BaseServer;
  void processPointCloudMessageAndInsert(
      const sensor_msgs::PointCloud2::Ptr& message,
      const voxblox::Transformation& transform,
      const bool freespace) override {
    const auto started = std::chrono::steady_clock::now();
    BaseServer::processPointCloudMessageAndInsert(message, transform, freespace);
    const double duration = std::chrono::duration<double, std::milli>(
        std::chrono::steady_clock::now() - started).count();
    if (!evaluation_profile_pub_) {
      evaluation_profile_pub_ = this->nh_private_.template advertise<std_msgs::Float64MultiArray>("native_map_profile", 10);
    }
    std_msgs::Float64MultiArray profile;
    profile.layout.dim.resize(1);
    profile.layout.dim[0].label = "scan_stamp,native_map_callback_ms,freespace";
    profile.data = {message->header.stamp.toSec(), duration, freespace ? 1.0 : 0.0};
    profile.layout.dim[0].size = profile.data.size();
    profile.layout.dim[0].stride = profile.data.size();
    evaluation_profile_pub_.publish(profile);
  }
 private:
  ros::Publisher evaluation_profile_pub_;
};
