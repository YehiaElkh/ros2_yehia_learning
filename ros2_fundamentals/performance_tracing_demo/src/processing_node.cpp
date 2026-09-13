#include <chrono>
#include <memory>
#include <thread>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class ProcessingNode : public rclcpp::Node{
    public:
    ProcessingNode()
    : Node("processing_node")
    {
        subscription_ =
        this->create_subscription<std_msgs::msg::String>(
            "/camera/frame",
            10,
            std::bind(
            &ProcessingNode::processFrame,
            this,
            std::placeholders::_1
            )
        );

        RCLCPP_INFO(
        this->get_logger(),
        "ProcessingNode started"
        );
    }

    private:

    void processFrame(
        const std_msgs::msg::String::SharedPtr message){
        const auto start =
        std::chrono::steady_clock::now();

        RCLCPP_INFO(
        this->get_logger(),
        "Processing %s",
        message->data.c_str()
        );

        std::this_thread::sleep_for(20ms);

        const auto end =
        std::chrono::steady_clock::now();

        const auto duration =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - start);

        RCLCPP_INFO(
        this->get_logger(),
        "Processing duration: %ld us",
        duration.count()
        );
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr
    subscription_;
};

int main(int argc, char * argv[]){
  rclcpp::init(argc, argv);
  auto node =
    std::make_shared<ProcessingNode>();

  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}