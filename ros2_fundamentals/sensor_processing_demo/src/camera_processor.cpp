#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"

class CameraProcessor : public rclcpp::Node{
    public:
    CameraProcessor() : Node("camera_processor"){
        subscription_ = create_subscription<sensor_msgs::msg::Image>(
        "/camera/image_raw",
        rclcpp::SensorDataQoS(),
        std::bind(
            &CameraProcessor::image_callback,
            this,
            std::placeholders::_1));

        publisher_ = create_publisher<sensor_msgs::msg::Image>(
        "/camera/image_processed",
        rclcpp::SensorDataQoS());
    }

    private:
    void image_callback(const sensor_msgs::msg::Image::SharedPtr msg){
        RCLCPP_INFO_THROTTLE(
        get_logger(),
        *get_clock(),
        2000,
        "Received image: %ux%u encoding=%s",
        msg->width,
        msg->height,
        msg->encoding.c_str());

        publisher_->publish(*msg);
    }

    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;

    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
};

int main(int argc, char ** argv){
  rclcpp::init(argc, argv);
  auto node = std::make_shared<CameraProcessor>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}