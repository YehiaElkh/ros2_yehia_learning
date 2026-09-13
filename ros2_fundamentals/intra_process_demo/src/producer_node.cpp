#include <chrono>
#include <memory>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class ProducerNode : public rclcpp::Node{
    public:
    explicit ProducerNode(const rclcpp::NodeOptions & options)
    : Node("producer_node", options)
    {
        publisher_ = this->create_publisher<std_msgs::msg::String>(
        "/intra_process/data",
        10
        );

        timer_ = this->create_wall_timer(
        1s,
        std::bind(&ProducerNode::publishMessage, this)
        );

        RCLCPP_INFO(
        this->get_logger(),
        "ProducerNode started"
        );
    }

    private:
    void publishMessage()
    {
        auto message = std::make_unique<std_msgs::msg::String>();

        message->data =
        "Message from ProducerNode: " +
        std::to_string(counter_);

        RCLCPP_INFO(
        this->get_logger(),
        "Publishing: %s",
        message->data.c_str()
        );

        publisher_->publish(std::move(message));

        ++counter_;
    }

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;

    rclcpp::TimerBase::SharedPtr timer_;

    std::size_t counter_{0};
};

#include "rclcpp_components/register_node_macro.hpp"

RCLCPP_COMPONENTS_REGISTER_NODE(ProducerNode)