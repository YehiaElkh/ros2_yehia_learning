#include <chrono>
#include <memory>
#include <thread>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;


class CallbackGroupDemoNode : public rclcpp::Node{
    public:

        CallbackGroupDemoNode() : Node("callback_group_demo_node"){
            exclusive_group_ =
                this->create_callback_group(
                    rclcpp::CallbackGroupType::MutuallyExclusive
                );

            reentrant_group_ =
                this->create_callback_group(
                    rclcpp::CallbackGroupType::Reentrant
                );

            timer_ =
                this->create_wall_timer(
                    1s,
                    std::bind(
                        &CallbackGroupDemoNode::slow_timer_callback,
                        this
                    ),
                    exclusive_group_
                );

            rclcpp::SubscriptionOptions subscription_options;

            subscription_options.callback_group =
                reentrant_group_;

            subscription_ =
                this->create_subscription<std_msgs::msg::String>(
                    "/callback_group/trigger",
                    10,
                    std::bind(
                        &CallbackGroupDemoNode::trigger_callback,
                        this,
                        std::placeholders::_1
                    ),
                    subscription_options
                );

            RCLCPP_INFO(
                this->get_logger(),
                "Callback group demo started"
            );
        }

    private:
        void slow_timer_callback(){
            RCLCPP_INFO(
                this->get_logger(),
                "SLOW CALLBACK START"
            );

            std::this_thread::sleep_for(
                5s
            );

            RCLCPP_INFO(
                this->get_logger(),
                "SLOW CALLBACK END"
            );
        }

        void trigger_callback(const std_msgs::msg::String::SharedPtr message){
            RCLCPP_INFO(
                this->get_logger(),
                "TRIGGER CALLBACK: %s",
                message->data.c_str()
            );
        }

        rclcpp::CallbackGroup::SharedPtr
            exclusive_group_;

        rclcpp::CallbackGroup::SharedPtr
            reentrant_group_;

        rclcpp::TimerBase::SharedPtr
            timer_;

        rclcpp::Subscription<
            std_msgs::msg::String>::SharedPtr
            subscription_;
};

int main(int argc,char * argv[]){
    rclcpp::init(argc, argv);
    auto node =
        std::make_shared<
            CallbackGroupDemoNode>();

    rclcpp::executors::MultiThreadedExecutor executor(
        rclcpp::ExecutorOptions(),
        2
    );

    executor.add_node(node);
    executor.spin();
    rclcpp::shutdown();
    return 0;
}