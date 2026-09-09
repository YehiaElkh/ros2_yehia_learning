#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include "std_msgs/msg/string.hpp"


class PerceptionComponent : public rclcpp::Node{
    public:

        explicit PerceptionComponent(
            const rclcpp::NodeOptions & options)
            : Node("perception_component", options)
        {
            subscription_ =
                this->create_subscription<
                    std_msgs::msg::String>(
                    "/camera/image",
                    10,
                    std::bind(
                        &PerceptionComponent::image_callback,
                        this,
                        std::placeholders::_1
                    )
                );

            RCLCPP_INFO(
                this->get_logger(),
                "Perception component started"
            );
        }


    private:

        void image_callback(
            const std_msgs::msg::String::SharedPtr message)
        {
            RCLCPP_INFO(
                this->get_logger(),
                "Perception received: %s",
                message->data.c_str()
            );

            RCLCPP_INFO(
                this->get_logger(),
                "Running perception processing..."
            );
        }


        rclcpp::Subscription<
            std_msgs::msg::String>::SharedPtr
            subscription_;
};


RCLCPP_COMPONENTS_REGISTER_NODE(
    PerceptionComponent)