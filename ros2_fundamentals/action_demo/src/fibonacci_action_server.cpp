#include <chrono>
#include <functional>
#include <memory>
#include <thread>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "example_interfaces/action/fibonacci.hpp"

using namespace std::chrono_literals;

class FibonacciActionServer : public rclcpp::Node{
    public:
        using Fibonacci = example_interfaces::action::Fibonacci;
        using GoalHandleFibonacci =
            rclcpp_action::ServerGoalHandle<Fibonacci>;
            
        FibonacciActionServer()
            : Node("fibonacci_action_server")
        {
            action_server_ =
                rclcpp_action::create_server<Fibonacci>(
                    this,
                    "/fibonacci",
                    std::bind(
                        &FibonacciActionServer::handle_goal,
                        this,
                        std::placeholders::_1,
                        std::placeholders::_2
                    ),
                    std::bind(
                        &FibonacciActionServer::handle_cancel,
                        this,
                        std::placeholders::_1
                    ),
                    std::bind(
                        &FibonacciActionServer::handle_accepted,
                        this,
                        std::placeholders::_1
                    )
                );
            RCLCPP_INFO(
                this->get_logger(),
                "Fibonacci Action Server is ready."
            );
        }

    private:
        rclcpp_action::GoalResponse handle_goal(
            const rclcpp_action::GoalUUID & uuid,
            std::shared_ptr<const Fibonacci::Goal> goal){
            (void)uuid;
            RCLCPP_INFO(
                this->get_logger(),
                "Received goal: order = %d",
                goal->order
            );

            if (goal->order <= 0){
                RCLCPP_WARN(
                    this->get_logger(),
                    "Goal rejected: order must be > 0."
                );
                return rclcpp_action::GoalResponse::REJECT;
            }
            RCLCPP_INFO(
                this->get_logger(),
                "Goal accepted."
            );
            return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
        }

        rclcpp_action::CancelResponse handle_cancel(
            const std::shared_ptr<GoalHandleFibonacci> goal_handle){
            (void)goal_handle;
            RCLCPP_INFO(
                this->get_logger(),
                "Cancel request received."
            );

            return rclcpp_action::CancelResponse::ACCEPT;
        }

        void handle_accepted(
            const std::shared_ptr<GoalHandleFibonacci> goal_handle){
            std::thread{
                std::bind(
                    &FibonacciActionServer::execute,
                    this,
                    std::placeholders::_1
                ),
                goal_handle
            }.detach();
        }

        void execute(
            const std::shared_ptr<GoalHandleFibonacci> goal_handle){
            RCLCPP_INFO(
                this->get_logger(),
                "Executing Fibonacci goal..."
            );

            const auto goal =
                goal_handle->get_goal();

            auto feedback =
                std::make_shared<Fibonacci::Feedback>();

                auto result =
                std::make_shared<Fibonacci::Result>();

            feedback->sequence.push_back(0);
            feedback->sequence.push_back(1);
            for (int i = 1; i < goal->order; ++i){
                if (goal_handle->is_canceling()){
                    result->sequence =
                        feedback->sequence;
                    goal_handle->canceled(result);
                    RCLCPP_INFO(
                        this->get_logger(),
                        "Goal canceled."
                    );
                    return;
                }

                const auto next =
                    feedback->sequence[i] +
                    feedback->sequence[i - 1];

                feedback->sequence.push_back(next);
                goal_handle->publish_feedback(feedback);
                RCLCPP_INFO(
                    this->get_logger(),
                    "Feedback published. Sequence size: %zu",
                    feedback->sequence.size()
                );
                std::this_thread::sleep_for(1s);
            }
            result->sequence = feedback->sequence;

            goal_handle->succeed(result);
            RCLCPP_INFO(
                this->get_logger(),
                "Goal succeeded."
            );
        }
        rclcpp_action::Server<Fibonacci>::SharedPtr
            action_server_;
};

int main(int argc, char * argv[]){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<FibonacciActionServer>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}