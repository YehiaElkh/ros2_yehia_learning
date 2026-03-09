#include "rclcpp/rclcpp.hpp"
#include "custom_msg_and_srv/msg/employee_salary.hpp"

class SubscriberCustomMsg : public rclcpp::Node
{
public:
    SubscriberCustomMsg() : Node("subscriber_custom_msg")
    {
        subscriber_ = this->create_subscription<custom_msg_and_srv::msg::EmployeeSalary>(
            "employee", 10,
            std::bind(&SubscriberCustomMsg::messageCallback, this, std::placeholders::_1)
        );
        RCLCPP_INFO(this->get_logger(), "Subscriber Custom Msg started !");
    }

private:
    void messageCallback(const custom_msg_and_srv::msg::EmployeeSalary::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "Received: %s %s - %.2f",
            msg->first_name.c_str(), msg->last_name.c_str(), msg->salary);
    }

    rclcpp::Subscription<custom_msg_and_srv::msg::EmployeeSalary>::SharedPtr subscriber_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SubscriberCustomMsg>());
    rclcpp::shutdown();
    return 0;
}