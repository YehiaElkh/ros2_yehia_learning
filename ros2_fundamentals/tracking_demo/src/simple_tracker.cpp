#include <cmath>
#include <limits>
#include <memory>
#include <vector>
#include <algorithm>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"

struct Detection{
  float x;
  float y;
};

struct Track{
  int id;
  float x;
  float y;
  int missed_frames;
};

class SimpleTracker : public rclcpp::Node{
public:
  SimpleTracker() : Node("simple_tracker"), next_id_(1){
    subscription_ =
      create_subscription<std_msgs::msg::Float32MultiArray>(
        "/detections",
        10,
        std::bind(
          &SimpleTracker::detection_callback,
          this,
          std::placeholders::_1));

    RCLCPP_INFO(
      get_logger(),
      "Simple tracker started");
  }

private:
  void detection_callback(const std_msgs::msg::Float32MultiArray::SharedPtr msg){
    std::vector<Detection> detections;

    for (size_t i = 0; i + 1 < msg->data.size(); i += 2){
      detections.push_back({
        msg->data[i],
        msg->data[i + 1]
      });
    }

    std::vector<bool> matched(
      tracks_.size(), false);

    for (const auto & detection : detections){
      float best_distance =
        std::numeric_limits<float>::max();

      int best_track = -1;

      for (size_t i = 0; i < tracks_.size(); ++i){
        if (matched[i]){
          continue;
        }
        const float dx = tracks_[i].x - detection.x;
        const float dy = tracks_[i].y - detection.y;
        const float distance = std::sqrt(dx * dx + dy * dy);
        if (distance < best_distance){
          best_distance = distance;
          best_track =
            static_cast<int>(i);
        }
      }

      const float association_threshold = 2.0f;

      if (best_track >= 0 && best_distance < association_threshold){
        auto & track = tracks_[best_track];

        track.x = detection.x;
        track.y = detection.y;
        track.missed_frames = 0;

        matched[best_track] = true;

        RCLCPP_INFO(
          get_logger(),
          "Track %d updated: (%.2f, %.2f)",
          track.id,
          track.x,
          track.y);
      }else{
        Track new_track;

        new_track.id = next_id_++;
        new_track.x = detection.x;
        new_track.y = detection.y;
        new_track.missed_frames = 0;

        tracks_.push_back(new_track);

        RCLCPP_INFO(
          get_logger(),
          "New track %d: (%.2f, %.2f)",
          new_track.id,
          new_track.x,
          new_track.y);
      }
    }

    for (size_t i = 0; i < tracks_.size(); ++i){
      if (!matched[i]){
        tracks_[i].missed_frames++;
      }
    }

    tracks_.erase(
      std::remove_if(
        tracks_.begin(),
        tracks_.end(),
        [](const Track & track)
        {
          return track.missed_frames > 5;
        }),
      tracks_.end());
  }

  rclcpp::Subscription<
    std_msgs::msg::Float32MultiArray>::SharedPtr
    subscription_;
  std::vector<Track> tracks_;
  int next_id_;
};

int main(int argc, char ** argv){
  rclcpp::init(argc, argv);
  auto node = std::make_shared<SimpleTracker>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}