#include "publisher_library/timed_publisher.hpp"

namespace publisher_library
{

TimedPublisher::TimedPublisher(
    rclcpp::Node::SharedPtr node,
    const std::string & topic,
    double period_seconds)
: node_(node), message_("Default message"), count_(0)
{
    publisher_ = node_->create_publisher<std_msgs::msg::String>(topic, 10);

    timer_ = node_->create_wall_timer(
        std::chrono::duration<double>(period_seconds),
        std::bind(&TimedPublisher::timerCallback, this)
    );

    RCLCPP_INFO(node_->get_logger(),
        "TimedPublisher created on topic '%s' with period %.2fs",
        topic.c_str(), period_seconds);
}

void TimedPublisher::start()
{
    if (timer_->is_canceled()) {
        timer_->reset();
        RCLCPP_INFO(node_->get_logger(), "TimedPublisher started !");
    }
}

void TimedPublisher::stop()
{
    timer_->cancel();
    RCLCPP_INFO(node_->get_logger(), "TimedPublisher stopped !");
}

void TimedPublisher::setMessage(const std::string & message)
{
    message_ = message;
    RCLCPP_INFO(node_->get_logger(), "Message changed to: '%s'", message_.c_str());
}

void TimedPublisher::timerCallback()
{
    auto msg = std_msgs::msg::String();
    msg.data = message_ + " [" + std::to_string(count_++) + "]";
    publisher_->publish(msg);
    RCLCPP_INFO(node_->get_logger(), "Publishing: '%s'", msg.data.c_str());
}

}  // namespace publisher_library
