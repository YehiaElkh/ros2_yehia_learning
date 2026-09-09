#include <chrono>
#include <memory>
#include <thread>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;


class ExecutorDemoNode : public rclcpp::Node{
    public:

        ExecutorDemoNode()
            : Node("executor_demo_node")
        {
            timer_ =
                this->create_wall_timer(
                    100ms,
                    std::bind(
                        &ExecutorDemoNode::timer_callback,
                        this
                    )
                );

            subscription_ =
                this->create_subscription<
                    std_msgs::msg::String>(
                    "/executor/trigger",
                    10,
                    std::bind(
                        &ExecutorDemoNode::trigger_callback,
                        this,
                        std::placeholders::_1
                    )
                );

            RCLCPP_INFO(
                this->get_logger(),
                "Executor demo node started"
            );
        }


    private:

        void timer_callback(){
            RCLCPP_INFO(
                this->get_logger(),
                "TIMER START"
            );

            std::this_thread::sleep_for(
                500ms
            );

            RCLCPP_INFO(
                this->get_logger(),
                "TIMER END"
            );
        }

        void trigger_callback(
            const std_msgs::msg::String::SharedPtr message)
        {
            RCLCPP_INFO(
                this->get_logger(),
                "TRIGGER CALLBACK: %s",
                message->data.c_str()
            );
        }

        rclcpp::TimerBase::SharedPtr timer_;

        rclcpp::Subscription<
            std_msgs::msg::String>::SharedPtr
            subscription_;
};


int main(int argc, char * argv[]){
    rclcpp::init(argc, argv);
    auto node =
        std::make_shared<ExecutorDemoNode>();

    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);
    executor.spin();
    rclcpp::shutdown();
    return 0;
}