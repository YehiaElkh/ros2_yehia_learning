#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"

using namespace std::chrono_literals;

class FaultMonitor : public rclcpp::Node{
    public:
    FaultMonitor()
    : Node("fault_monitor"),
        heartbeat_received_(false)
    {
        heartbeat_sub_ =
        create_subscription<std_msgs::msg::Bool>(
            "/system/heartbeat",
            10,
            std::bind(
            &FaultMonitor::heartbeat_callback,
            this,
            std::placeholders::_1));

        monitor_timer_ =
        create_wall_timer(
            100ms,
            std::bind(
            &FaultMonitor::monitor_callback,
            this));
    }

    private:
    void heartbeat_callback(
        const std_msgs::msg::Bool::SharedPtr msg){
        if (msg->data){
        last_heartbeat_ =
            this->now();

        heartbeat_received_ = true;
        }
    }

    void monitor_callback(){
        if (!heartbeat_received_){
        RCLCPP_WARN(
            get_logger(),
            "Waiting for heartbeat...");

        return;
        }

        const auto now = this->now();
        const double elapsed =
        (now - last_heartbeat_).seconds();

        if (elapsed > 0.5){
        RCLCPP_ERROR(
            get_logger(),
            "FAULT: heartbeat timeout");

        fault_active_ = true;
        }else{
        if (fault_active_){
            RCLCPP_INFO(
            get_logger(),
            "RECOVERY: heartbeat restored");
        }

        fault_active_ = false;

        RCLCPP_INFO(
            get_logger(),
            "System healthy");
        }
    }

    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr
        heartbeat_sub_;

    rclcpp::TimerBase::SharedPtr
        monitor_timer_;
    rclcpp::Time last_heartbeat_;
    bool heartbeat_received_;
    bool fault_active_{false};
};

int main(int argc, char ** argv){
  rclcpp::init(argc, argv);
  auto node = std::make_shared<FaultMonitor>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}