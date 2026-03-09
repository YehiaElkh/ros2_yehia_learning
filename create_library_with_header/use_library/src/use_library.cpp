#include "rclcpp/rclcpp.hpp"
#include "publisher_library/timed_publisher.hpp"

class UseLibrary : public rclcpp::Node
{
public:
    UseLibrary() : Node("use_library")
    {
        RCLCPP_INFO(this->get_logger(), "UseLibrary node started !");

        publisher1_ = std::make_shared<publisher_library::TimedPublisher>(
            shared_from_this(),
            "topic_fast",
            0.5
        );
        publisher1_->setMessage("Fast message");

        publisher2_ = std::make_shared<publisher_library::TimedPublisher>(
            shared_from_this(),
            "topic_slow",
            2.0
        );
        publisher2_->setMessage("Slow message");
    }

private:
    std::shared_ptr<publisher_library::TimedPublisher> publisher1_;
    std::shared_ptr<publisher_library::TimedPublisher> publisher2_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<UseLibrary>());
    rclcpp::shutdown();
    return 0;
}