#include <cmath>
#include <memory>
#include "rclcpp/rclcpp.hpp"

class LocalizationNode : public rclcpp::Node{
    public:
    LocalizationNode()
    : Node("localization_node"),
        estimated_x_(0.0),
        odometry_x_(0.0),
        measurement_x_(0.0)
    {
        timer_ = create_wall_timer(
        std::chrono::milliseconds(100),
        std::bind(
            &LocalizationNode::update,
            this));

        RCLCPP_INFO(
        get_logger(),
        "Localization node started");
    }

    private:
    void update(){
        // Simulated odometry motion
        odometry_x_ += 0.1;

        // Prediction from odometry
        estimated_x_ = odometry_x_;

        // Simulated absolute measurement
        measurement_x_ =
        10.0 + 0.2 * std::sin(
            odometry_x_);

        // Simple correction
        constexpr double gain = 0.2;

        estimated_x_ =
        estimated_x_ +
        gain *
        (measurement_x_ - estimated_x_);

        RCLCPP_INFO_THROTTLE(
        get_logger(),
        *get_clock(),
        1000,
        "Odometry: %.2f | Measurement: %.2f | Estimated pose: %.2f",
        odometry_x_,
        measurement_x_,
        estimated_x_);
    }

    double estimated_x_;
    double odometry_x_;
    double measurement_x_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv){
  rclcpp::init(argc, argv);
  auto node = std::make_shared<LocalizationNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}