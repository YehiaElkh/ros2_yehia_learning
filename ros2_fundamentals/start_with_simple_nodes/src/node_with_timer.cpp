#include "rclcpp/rclcpp.hpp"

class MyNode : public rclcpp::Node
{
public:
    MyNode() : Node("node_with_timer"), count_(0)
    {
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(500),
            std::bind(&MyNode::timerCallback, this)
        );
        RCLCPP_INFO(this->get_logger(), "Node started !");
    }

private:
    void timerCallback()
    {
        count_++;
        RCLCPP_INFO(this->get_logger(), "Hello n:%d", count_);
    }

    rclcpp::TimerBase::SharedPtr timer_;
    int count_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MyNode>());
    rclcpp::shutdown();
    return 0;
}