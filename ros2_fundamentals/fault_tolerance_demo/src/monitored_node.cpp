#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"

using namespace std::chrono_literals;

class MonitoredNode : public rclcpp::Node{
    public:
    MonitoredNode()
    : Node("monitored_node"){
        heartbeat_pub_ =
        create_publisher<std_msgs::msg::Bool>(
            "/system/heartbeat",
            10);

        timer_ =
        create_wall_timer(
            100ms,
            std::bind(
            &MonitoredNode::heartbeat_callback,
            this));
    }

    private:

    void heartbeat_callback(){
        std_msgs::msg::Bool msg;
        msg.data = true;

        heartbeat_pub_->publish(msg);

        RCLCPP_INFO(
        get_logger(),
        "Heartbeat");
    }

    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr
        heartbeat_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv){
  rclcpp::init(argc, argv);
  auto node = std::make_shared<MonitoredNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}