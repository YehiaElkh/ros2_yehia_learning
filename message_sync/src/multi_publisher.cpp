#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int32.hpp"

// ============================================================
// Ce nœud publie sur DEUX topics différents
// pour simuler deux capteurs
// ============================================================

class MultiPublisher : public rclcpp::Node
{
public:
    MultiPublisher() : Node("multi_publisher"), count_(0)
    {
        // Publisher 1 : simule une caméra → publie du texte
        publisher1_ = this->create_publisher<std_msgs::msg::String>("camera_data", 10);

        // Publisher 2 : simule un LiDAR → publie un entier
        publisher2_ = this->create_publisher<std_msgs::msg::Int32>("lidar_data", 10);

        // Timer : publier les deux topics toutes les 500ms
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(500),
            std::bind(&MultiPublisher::timerCallback, this)
        );

        RCLCPP_INFO(this->get_logger(), "Multi Publisher started !");
    }

private:
    void timerCallback()
    {
        // Message caméra
        auto msg1 = std_msgs::msg::String();
        msg1.data = "Camera frame n°" + std::to_string(count_);

        // Message LiDAR
        auto msg2 = std_msgs::msg::Int32();
        msg2.data = count_ * 10;

        count_++;

        // Publier les deux messages
        publisher1_->publish(msg1);
        publisher2_->publish(msg2);

        RCLCPP_INFO(this->get_logger(),
            "Published: '%s' | lidar: %d",
            msg1.data.c_str(), msg2.data);
    }

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher1_;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr publisher2_;
    rclcpp::TimerBase::SharedPtr timer_;
    int count_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MultiPublisher>());
    rclcpp::shutdown();
    return 0;
}