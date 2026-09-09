#include <chrono>
#include <memory>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;


class CameraComponent : public rclcpp::Node{
    public:

        explicit CameraComponent(
            const rclcpp::NodeOptions & options)
            : Node("camera_component", options)
            {
            publisher_ =
                this->create_publisher<std_msgs::msg::String>(
                    "/camera/image",
                    10
                );

            timer_ =
                this->create_wall_timer(
                    1s,
                    std::bind(
                        &CameraComponent::publish_image,
                        this
                    )
                );

            RCLCPP_INFO(
                this->get_logger(),
                "Camera component started"
            );
        }


    private:

        void publish_image(){
            std_msgs::msg::String message;

            message.data =
                "Camera frame received";

            publisher_->publish(message);

            RCLCPP_INFO(
                this->get_logger(),
                "Published camera frame"
            );
        }


        rclcpp::Publisher<
            std_msgs::msg::String>::SharedPtr
            publisher_;

        rclcpp::TimerBase::SharedPtr timer_;
};


RCLCPP_COMPONENTS_REGISTER_NODE(CameraComponent)