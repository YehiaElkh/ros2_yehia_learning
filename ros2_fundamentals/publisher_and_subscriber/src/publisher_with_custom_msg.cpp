#include "rclcpp/rclcpp.hpp"
#include "custom_msg_and_srv/msg/employee_salary.hpp"

class PublisherCustomMsg : public rclcpp::Node
{
public:
    PublisherCustomMsg() : Node("publisher_custom_msg"), count_(0)
    {
        publisher_ = this->create_publisher<custom_msg_and_srv::msg::EmployeeSalary>("employee", 10);
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(500),
            std::bind(&PublisherCustomMsg::timerCallback, this)
        );
        RCLCPP_INFO(this->get_logger(), "Publisher Custom Msg started !");
    }

private:
    void timerCallback()
    {
        auto msg = custom_msg_and_srv::msg::EmployeeSalary();
        msg.first_name = "John";
        msg.last_name  = "Doe";
        msg.salary     = 3000.0 + count_ * 100.0;
        count_++;
        RCLCPP_INFO(this->get_logger(), "Publishing: %s %s - %.2f",
            msg.first_name.c_str(), msg.last_name.c_str(), msg.salary);
        publisher_->publish(msg);
    }

    rclcpp::Publisher<custom_msg_and_srv::msg::EmployeeSalary>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    int count_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PublisherCustomMsg>());
    rclcpp::shutdown();
    return 0;
}