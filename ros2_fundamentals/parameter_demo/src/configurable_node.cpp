#include <memory>
#include <vector>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "rcl_interfaces/msg/set_parameters_result.hpp"
#include "std_msgs/msg/string.hpp"

class PerceptionNode : public rclcpp::Node{
    public:
        PerceptionNode()
            : Node("perception_node"){
            confidence_threshold_ =
                this->declare_parameter<double>(
                    "confidence_threshold",
                    0.5
                );

            max_distance_ =
                this->declare_parameter<double>(
                    "max_distance",
                    50.0
                );

            processing_rate_ =
                this->declare_parameter<int>(
                    "processing_rate",
                    10
                );

            status_publisher_ =
                this->create_publisher<std_msgs::msg::String>(
                    "camera/status",
                    10
                );

            parameter_callback_handle_ =
                this->add_on_set_parameters_callback(
                    std::bind(
                        &PerceptionNode::parameters_callback,
                        this,
                        std::placeholders::_1
                    )
                );

            timer_ =
                this->create_wall_timer(
                    std::chrono::milliseconds(
                        1000 / processing_rate_
                    ),
                    std::bind(
                        &PerceptionNode::process,
                        this
                    )
                );
            RCLCPP_INFO(
                this->get_logger(),
                "Perception node started"
            );

            RCLCPP_INFO(
                this->get_logger(),
                "Confidence threshold: %.2f",
                confidence_threshold_
            );

            RCLCPP_INFO(
                this->get_logger(),
                "Max distance: %.2f m",
                max_distance_
            );

            RCLCPP_INFO(
                this->get_logger(),
                "Processing rate: %d Hz",
                processing_rate_
            );
        }

    private:

        rcl_interfaces::msg::SetParametersResult
        parameters_callback(
            const std::vector<rclcpp::Parameter> & parameters){
            rcl_interfaces::msg::SetParametersResult result;

            result.successful = true;
            result.reason = "Parameters accepted";

            for (const auto & parameter : parameters){
                if (parameter.get_name() ==
                    "confidence_threshold"){
                    const double value =
                        parameter.as_double();

                    if (value < 0.0 || value > 1.0){
                        result.successful = false;
                        result.reason =
                            "confidence_threshold must be "
                            "between 0.0 and 1.0";

                        return result;
                    }
                }

                if (parameter.get_name() ==
                    "max_distance"){
                    const double value =
                        parameter.as_double();

                    if (value <= 0.0){
                        result.successful = false;
                        result.reason =
                            "max_distance must be greater than 0";

                        return result;
                    }
                }

                if (parameter.get_name() ==
                    "processing_rate"){
                    const int value =
                        parameter.as_int();

                    if (value <= 0){
                        result.successful = false;
                        result.reason =
                            "processing_rate must be greater than 0";

                        return result;
                    }
                }
            }

            for (const auto & parameter : parameters){
                if (parameter.get_name() ==
                    "confidence_threshold"){
                    confidence_threshold_ =
                        parameter.as_double();

                    RCLCPP_INFO(
                        this->get_logger(),
                        "New confidence threshold: %.2f",
                        confidence_threshold_
                    );
                }else if (parameter.get_name() ==
                        "max_distance"){
                    max_distance_ =
                        parameter.as_double();

                    RCLCPP_INFO(
                        this->get_logger(),
                        "New max distance: %.2f m",
                        max_distance_
                    );
                }else if (parameter.get_name() ==
                        "processing_rate"){
                    processing_rate_ =
                        parameter.as_int();

                    RCLCPP_INFO(
                        this->get_logger(),
                        "New processing rate: %d Hz",
                        processing_rate_
                    );
                }
            }

            return result;
        }
        void process(){
            RCLCPP_INFO_THROTTLE(
                this->get_logger(),
                *this->get_clock(),
                2000,
                "Processing perception | "
                "threshold=%.2f | max_distance=%.2f m | rate=%d Hz",
                confidence_threshold_,
                max_distance_,
                processing_rate_
            );

            std_msgs::msg::String message;

            message.data =
                "Perception node processing";

            status_publisher_->publish(message);
        }
        double confidence_threshold_;
        double max_distance_;
        int processing_rate_;

        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr
            status_publisher_;

        rclcpp::TimerBase::SharedPtr
            timer_;

        rclcpp::node_interfaces::
            OnSetParametersCallbackHandle::SharedPtr
            parameter_callback_handle_;
};


int main(int argc, char * argv[]){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<PerceptionNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}