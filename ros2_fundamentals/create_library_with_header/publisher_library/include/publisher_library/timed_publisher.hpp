#ifndef PUBLISHER_LIBRARY__TIMED_PUBLISHER_HPP_
#define PUBLISHER_LIBRARY__TIMED_PUBLISHER_HPP_

#include <string>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

namespace publisher_library
{

class TimedPublisher
{
public:
    TimedPublisher(
        rclcpp::Node::SharedPtr node,
        const std::string & topic,
        double period_seconds
    );

    void start();
    void stop();
    void setMessage(const std::string & message);

private:
    void timerCallback();

    rclcpp::Node::SharedPtr node_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    std::string message_;
    int count_;
};

}  // namespace publisher_library

#endif
