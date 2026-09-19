#include <cmath>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/point_cloud2.hpp"

class LidarProcessor : public rclcpp::Node{
    public:
    LidarProcessor() : Node("lidar_processor"){
        subscription_ = create_subscription<sensor_msgs::msg::PointCloud2>(
        "/lidar/points",
        rclcpp::SensorDataQoS(),
        std::bind(
            &LidarProcessor::pointcloud_callback,
            this,
            std::placeholders::_1));

        publisher_ = create_publisher<sensor_msgs::msg::PointCloud2>(
        "/lidar/points_processed",
        rclcpp::SensorDataQoS());
    }

    private:
    void pointcloud_callback(const sensor_msgs::msg::PointCloud2::SharedPtr msg){
        const std::size_t point_count =
        static_cast<std::size_t>(msg->width) *
        static_cast<std::size_t>(msg->height);

        RCLCPP_INFO_THROTTLE(
        get_logger(),
        *get_clock(),
        2000,
        "Received point cloud: %zu points",
        point_count);

        publisher_->publish(*msg);
    }

    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr subscription_;

    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr publisher_;
};

int main(int argc, char ** argv){
  rclcpp::init(argc, argv);
  auto node = std::make_shared<LidarProcessor>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}