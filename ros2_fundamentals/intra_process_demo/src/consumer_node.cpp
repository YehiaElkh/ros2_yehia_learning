#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class ConsumerNode : public rclcpp::Node{
    public:
    explicit ConsumerNode(const rclcpp::NodeOptions & options)
    : Node("consumer_node", options)
    {
        subscription_ =
        this->create_subscription<std_msgs::msg::String>(
            "/intra_process/data",
            10,
            std::bind(
            &ConsumerNode::messageCallback,
            this,
            std::placeholders::_1
            )
        );

        RCLCPP_INFO(
        this->get_logger(),
        "ConsumerNode started"
        );
    }

    private:
    void messageCallback(
        std_msgs::msg::String::UniquePtr message)
    {
        RCLCPP_INFO(
        this->get_logger(),
        "Received: %s",
        message->data.c_str()
        );
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr
        subscription_;
};

#include "rclcpp_components/register_node_macro.hpp"

RCLCPP_COMPONENTS_REGISTER_NODE(ConsumerNode)