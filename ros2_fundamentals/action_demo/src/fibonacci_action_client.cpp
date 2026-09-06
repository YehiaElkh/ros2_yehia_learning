#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "example_interfaces/action/fibonacci.hpp"

using namespace std::chrono_literals;

class FibonacciActionClient : public rclcpp::Node{
    public:
        using Fibonacci = example_interfaces::action::Fibonacci;
        using GoalHandleFibonacci =
            rclcpp_action::ClientGoalHandle<Fibonacci>;
        FibonacciActionClient()
            : Node("fibonacci_action_client"){
            client_ =
                rclcpp_action::create_client<Fibonacci>(
                    this,
                    "/fibonacci"
                );
        }

        void send_goal(int order){
            if (!client_->wait_for_action_server(5s)){
                RCLCPP_ERROR(
                    this->get_logger(),
                    "Fibonacci Action Server is not available."
                );

                return;
            }
            auto goal_msg = Fibonacci::Goal();

            goal_msg.order = order;
            RCLCPP_INFO(
                this->get_logger(),
                "Sending goal: order = %d",
                order
            );
            rclcpp_action::Client<Fibonacci>::SendGoalOptions options;
            options.goal_response_callback =
                std::bind(
                    &FibonacciActionClient::goal_response_callback,
                    this,
                    std::placeholders::_1
                );
            options.feedback_callback =
                std::bind(
                    &FibonacciActionClient::feedback_callback,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2
                );
            options.result_callback =
                std::bind(
                    &FibonacciActionClient::result_callback,
                    this,
                    std::placeholders::_1
                );
            client_->async_send_goal(
                goal_msg,
                options
            );
        }

    private:
        void goal_response_callback(
            const GoalHandleFibonacci::SharedPtr & goal_handle){
            if (!goal_handle){
                RCLCPP_ERROR(
                    this->get_logger(),
                    "Goal was rejected by the server."
                );

                return;
            }

            RCLCPP_INFO(
                this->get_logger(),
                "Goal accepted by the server."
            );
        }

        void feedback_callback(
            GoalHandleFibonacci::SharedPtr,
            const std::shared_ptr<const Fibonacci::Feedback>
                feedback){
            std::string sequence;
            for (const auto value :
                feedback->sequence){
                sequence +=
                    std::to_string(value) + " ";
            }

            RCLCPP_INFO(
                this->get_logger(),
                "Feedback: %s",
                sequence.c_str()
            );
        }

        void result_callback(
            const GoalHandleFibonacci::WrappedResult & result){

            if (result.code ==
                rclcpp_action::ResultCode::SUCCEEDED){
                std::string sequence;
                for (const auto value :
                    result.result->sequence){
                    sequence +=
                        std::to_string(value) + " ";
                }
                RCLCPP_INFO(
                    this->get_logger(),
                    "Final Result: %s",
                    sequence.c_str()
                );
            }else if (
                result.code ==
                rclcpp_action::ResultCode::CANCELED){
                RCLCPP_WARN(
                    this->get_logger(),
                    "Goal was canceled."
                );
            }else if (
                result.code ==
                rclcpp_action::ResultCode::ABORTED){
                RCLCPP_ERROR(
                    this->get_logger(),
                    "Goal was aborted."
                );
            }else{
                RCLCPP_ERROR(
                    this->get_logger(),
                    "Unknown result status."
                );
            }
            rclcpp::shutdown();
        }
        rclcpp_action::Client<Fibonacci>::SharedPtr
            client_;
};

int main(int argc, char * argv[]){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<FibonacciActionClient>();
    node->send_goal(6);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}