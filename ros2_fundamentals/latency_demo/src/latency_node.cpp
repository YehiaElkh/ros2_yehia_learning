#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/header.hpp"

using namespace std::chrono_literals;

class LatencyNode : public rclcpp::Node{
    public:
    LatencyNode() : Node("latency_node"){
        publisher_ =
        create_publisher<std_msgs::msg::Header>(
            "/latency/data",
            10);

        timer_ = create_wall_timer(
        100ms,
        std::bind(
            &LatencyNode::publish_data,
        this));
  }

private:

  void publish_data(){
    std_msgs::msg::Header msg;
    msg.stamp = get_clock()->now();
    publisher_->publish(msg);
  }

  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::Header>::SharedPtr publisher_;
};


class ProcessingNode : public rclcpp::Node{
    public:
    ProcessingNode() : Node("processing_node")
    {
        subscription_ =
        create_subscription<std_msgs::msg::Header>(
            "/latency/data",
            10,
            std::bind(
            &ProcessingNode::callback,
            this,
            std::placeholders::_1));
    }

    private:

    void callback(const std_msgs::msg::Header::SharedPtr msg){
        const auto now = get_clock()->now();

        const auto latency = now - rclcpp::Time(msg->stamp);

        RCLCPP_INFO(
        get_logger(),
        "Latency: %.3f ms",
        latency.seconds() * 1000.0);
    }

    rclcpp::Subscription<std_msgs::msg::Header>::SharedPtr subscription_;
};


int main(int argc, char ** argv){
  rclcpp::init(argc, argv);
  auto publisher = std::make_shared<LatencyNode>();
  auto processor = std::make_shared<ProcessingNode>();
  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(publisher);
  executor.add_node(processor);
  executor.spin();
  rclcpp::shutdown();
  return 0;
}