#include <chrono>
#include <cstdint>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class CameraNode : public rclcpp::Node{
    public:
    CameraNode()
    : Node("camera_node")
    {
        publisher_ =
        this->create_publisher<std_msgs::msg::String>(
            "/camera/frame",
            10
        );

        timer_ =
        this->create_wall_timer(
            100ms,
            std::bind(
            &CameraNode::publishFrame,
            this
            )
        );

        RCLCPP_INFO(
        this->get_logger(),
        "CameraNode started"
        );
    }

    private:

    void publishFrame(){
        auto message =
        std::make_unique<std_msgs::msg::String>();

        message->data =
        "frame_" + std::to_string(frame_id_);

        publisher_->publish(
        std::move(message)
        );

        ++frame_id_;
    }

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr
        publisher_;

    rclcpp::TimerBase::SharedPtr timer_;

    std::uint64_t frame_id_{0};
};

int main(int argc, char * argv[]){
  rclcpp::init(argc, argv);
  auto node =
    std::make_shared<CameraNode>();

  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}