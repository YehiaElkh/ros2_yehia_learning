#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class PublisherNode : public rclcpp::Node{
    public:
        PublisherNode() : Node("publisher_node"){
            publisher_ = this->create_publisher<std_msgs::msg::String>(
                "robot/status",
                10
            );
            timer_ = this->create_wall_timer(
                1s,
                std::bind(
                    &PublisherNode::publish_status,
                    this
                )
            );
        }
    private:
        void publish_status(){
            std_msgs::msg::String message;
            message.data = "Robot is running";
            publisher_->publish(message);
            RCLCPP_INFO(
                this->get_logger(), "Published: %s",
                message.data.c_str()
            );
        }
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[]){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<PublisherNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}