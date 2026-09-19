#include <chrono>
#include <cstdint>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"

using namespace std::chrono_literals;

class RealtimeControlNode : public rclcpp::Node{
    public:
    RealtimeControlNode()
    : Node("realtime_control_node"),
        cycle_count_(0)
    {
        command_pub_ =
        create_publisher<std_msgs::msg::Float64>(
            "/control/command",
            10);

        timer_ = create_wall_timer(
        10ms,
        std::bind(
            &RealtimeControlNode::control_loop,
            this));

        RCLCPP_INFO(
        get_logger(),
        "Real-time control node started at 100 Hz");
    }

    private:

    void control_loop(){
        const auto start = std::chrono::steady_clock::now();
        // Simulated control computation
        double command = 0.5;

        std_msgs::msg::Float64 msg;
        msg.data = command;
        command_pub_->publish(msg);
        cycle_count_++;

        const auto end = std::chrono::steady_clock::now();
        const auto execution_time =
        std::chrono::duration_cast<std::chrono::microseconds>(
            end - start);

        // Log only occasionally.
        if (cycle_count_ % 100 == 0){
        RCLCPP_INFO(
            get_logger(),
            "Control execution time: %ld us",
            execution_time.count());
        }
    }

    rclcpp::TimerBase::SharedPtr timer_;

    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr
        command_pub_;

    std::uint64_t cycle_count_;
};

int main(int argc, char ** argv){
  rclcpp::init(argc, argv);
  auto node = std::make_shared<RealtimeControlNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}