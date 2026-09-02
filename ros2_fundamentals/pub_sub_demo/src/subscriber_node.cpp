#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class SubscriberNode : public rclcpp::Node{
    public:
        SubscriberNode() : Node("subscriber_node"){
            subscription_ = this->create_subscription<std_msgs::msg::String>(
                "robot/status",
                10,
                std::bind(
                    &SubscriberNode::topic_callback,
                    this,
                    std::placeholders::_1
                )
            );
        }
    private:
        void topic_callback(const std_msgs::msg::String::SharedPtr message){
            RCLCPP_INFO(
                this->get_logger(), "Received: %s",
                message->data.c_str()
            );
        }
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char *argv[]){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SubscriberNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}