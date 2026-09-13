#include <atomic>
#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class ThreadSafetyDemoNode : public rclcpp::Node{
    public:
        ThreadSafetyDemoNode()
            : Node("thread_safety_demo_node"),
            unsafe_counter_(0),
            mutex_counter_(0),
            atomic_counter_(0){
            worker_group_ =
                this->create_callback_group(
                    rclcpp::CallbackGroupType::Reentrant
                );

            timer_group_ =
                this->create_callback_group(
                    rclcpp::CallbackGroupType::MutuallyExclusive
                );

            timer_ =
                this->create_wall_timer(
                    2s,
                    std::bind(
                        &ThreadSafetyDemoNode::timer_callback,
                        this
                    ),
                    timer_group_
                );

            rclcpp::SubscriptionOptions subscription_options;

            subscription_options.callback_group =
                worker_group_;

            command_subscription_ =
                this->create_subscription<std_msgs::msg::String>(
                    "/thread_safety/command",
                    10,
                    std::bind(
                        &ThreadSafetyDemoNode::command_callback,
                        this,
                        std::placeholders::_1
                    ),
                    subscription_options
                );

            RCLCPP_INFO(
                this->get_logger(),
                "=============================================="
            );

            RCLCPP_INFO(
                this->get_logger(),
                "Thread Safety Demo started"
            );

            RCLCPP_INFO(
                this->get_logger(),
                "Commands:"
            );

            RCLCPP_INFO(
                this->get_logger(),
                "  unsafe  -> run unsafe experiment"
            );

            RCLCPP_INFO(
                this->get_logger(),
                "  mutex   -> run mutex experiment"
            );

            RCLCPP_INFO(
                this->get_logger(),
                "  atomic  -> run atomic experiment"
            );

            RCLCPP_INFO(
                this->get_logger(),
                "  all     -> run all experiments"
            );

            RCLCPP_INFO(
                this->get_logger(),
                "  reset   -> reset counters"
            );

            RCLCPP_INFO(
                this->get_logger(),
                "=============================================="
            );
        }

    private:
        void run_unsafe_experiment(){
            RCLCPP_INFO(
                this->get_logger(),
                "[UNSAFE] Starting experiment..."
            );

            unsafe_counter_ = 0;
            constexpr int iterations = 1'000'000;

            std::thread thread_1(
                [this, iterations](){
                    for (int i = 0; i < iterations; ++i)
                    {
                        ++unsafe_counter_;
                    }
                }
            );

            std::thread thread_2(
                [this, iterations](){
                    for (int i = 0; i < iterations; ++i)
                    {
                        ++unsafe_counter_;
                    }
                }
            );

            thread_1.join();
            thread_2.join();

            const int expected =
                iterations * 2;

            RCLCPP_INFO(
                this->get_logger(),
                "[UNSAFE] Expected: %d",
                expected
            );

            RCLCPP_INFO(
                this->get_logger(),
                "[UNSAFE] Actual:   %d",
                unsafe_counter_
            );

            RCLCPP_INFO(
                this->get_logger(),
                "[UNSAFE] Difference: %d",
                expected - unsafe_counter_
            );
        }

        void run_mutex_experiment(){
            RCLCPP_INFO(
                this->get_logger(),
                "[MUTEX] Starting experiment..."
            );

            mutex_counter_ = 0;
            constexpr int iterations = 1'000'000;

            std::thread thread_1(
                [this, iterations](){
                    for (int i = 0; i < iterations; ++i)
                    {
                        std::lock_guard<std::mutex> lock(
                            counter_mutex_
                        );

                        ++mutex_counter_;
                    }
                }
            );

            std::thread thread_2(
                [this, iterations](){
                    for (int i = 0; i < iterations; ++i)
                    {
                        std::lock_guard<std::mutex> lock(
                            counter_mutex_
                        );

                        ++mutex_counter_;
                    }
                }
            );

            thread_1.join();
            thread_2.join();

            const int expected =
                iterations * 2;

            RCLCPP_INFO(
                this->get_logger(),
                "[MUTEX] Expected: %d",
                expected
            );

            RCLCPP_INFO(
                this->get_logger(),
                "[MUTEX] Actual:   %d",
                mutex_counter_
            );

            RCLCPP_INFO(
                this->get_logger(),
                "[MUTEX] Difference: %d",
                expected - mutex_counter_
            );
        }

        void run_atomic_experiment(){
            RCLCPP_INFO(
                this->get_logger(),
                "[ATOMIC] Starting experiment..."
            );

            atomic_counter_.store(0);
            constexpr int iterations = 1'000'000;

            std::thread thread_1(
                [this, iterations](){
                    for (int i = 0; i < iterations; ++i)
                    {
                        atomic_counter_.fetch_add(1);
                    }
                }
            );

            std::thread thread_2(
                [this, iterations](){
                    for (int i = 0; i < iterations; ++i)
                    {
                        atomic_counter_.fetch_add(1);
                    }
                }
            );

            thread_1.join();
            thread_2.join();

            const int expected =
                iterations * 2;

            const int actual =
                atomic_counter_.load();

            RCLCPP_INFO(
                this->get_logger(),
                "[ATOMIC] Expected: %d",
                expected
            );

            RCLCPP_INFO(
                this->get_logger(),
                "[ATOMIC] Actual:   %d",
                actual
            );

            RCLCPP_INFO(
                this->get_logger(),
                "[ATOMIC] Difference: %d",
                expected - actual
            );
        }

        void command_callback(
            const std_msgs::msg::String::SharedPtr message){
            const std::string command =
                message->data;

            RCLCPP_INFO(
                this->get_logger(),
                "Received command: %s",
                command.c_str()
            );
            if (command == "unsafe"){
                run_unsafe_experiment();
            }else if (command == "mutex"){
                run_mutex_experiment();
            }else if (command == "atomic"){
                run_atomic_experiment();
            }else if (command == "all"){
                run_unsafe_experiment();
                run_mutex_experiment();
                run_atomic_experiment();
            }else if (command == "reset"){
                reset_counters();
            }else{
                RCLCPP_WARN(
                    this->get_logger(),
                    "Unknown command: %s",
                    command.c_str()
                );
            }
        }

        void timer_callback(){
            const int atomic_value =
                atomic_counter_.load();

            RCLCPP_INFO(
                this->get_logger(),
                "[TIMER] atomic_counter = %d",
                atomic_value
            );
        }

        void reset_counters(){
            unsafe_counter_ = 0;
            {
                std::lock_guard<std::mutex> lock(
                    counter_mutex_
                );

                mutex_counter_ = 0;
            }

            atomic_counter_.store(0);
            RCLCPP_INFO(
                this->get_logger(),
                "Counters reset."
            );
        }

    private:

        rclcpp::CallbackGroup::SharedPtr worker_group_;

        rclcpp::CallbackGroup::SharedPtr timer_group_;

        rclcpp::TimerBase::SharedPtr timer_;

        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr
            command_subscription_;

        int unsafe_counter_;
        int mutex_counter_;
        std::mutex counter_mutex_;
        std::atomic<int> atomic_counter_;
};

int main(int argc, char * argv[]){
    rclcpp::init(argc, argv);
    auto node =
        std::make_shared<ThreadSafetyDemoNode>();

    rclcpp::executors::MultiThreadedExecutor executor(
        rclcpp::ExecutorOptions(),
        2
    );

    executor.add_node(node);
    executor.spin();
    rclcpp::shutdown();
    return 0;
}