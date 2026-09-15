#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "example_interfaces/action/fibonacci.hpp"

using namespace std::chrono_literals;


class AdvancedActionClient : public rclcpp::Node{
    public:

    using Fibonacci =
        example_interfaces::action::Fibonacci;

    using GoalHandle =
        rclcpp_action::ClientGoalHandle<Fibonacci>;

    AdvancedActionClient()
    : Node("advanced_action_client")
    {
        client_ =
        rclcpp_action::create_client<Fibonacci>(
            this,
            "advanced_fibonacci");

        RCLCPP_INFO(
        get_logger(),
        "Advanced Action Client started");
    }

    void send_goal(int order){
        if (!client_->wait_for_action_server(5s)){
        RCLCPP_ERROR(
            get_logger(),
            "Action server not available");

        return;
        }

        Fibonacci::Goal goal;

        goal.order = order;

        RCLCPP_INFO(
        get_logger(),
        "[CLIENT] Sending goal order=%d",
        order);

        rclcpp_action::Client<Fibonacci>::SendGoalOptions
        options;

        options.feedback_callback =
        [this](
            GoalHandle::SharedPtr,
            const std::shared_ptr<const Fibonacci::Feedback>
            feedback)
        {
            RCLCPP_INFO(
            get_logger(),
            "[CLIENT] Feedback: %zu values",
            feedback->sequence.size());
        };

        options.result_callback =
        [this](
            const GoalHandle::WrappedResult & result)
        {
            switch (result.code){

            case rclcpp_action::ResultCode::SUCCEEDED:

                RCLCPP_INFO(
                get_logger(),
                "[CLIENT] Result: SUCCEEDED");

                break;

            case rclcpp_action::ResultCode::CANCELED:

                RCLCPP_WARN(
                get_logger(),
                "[CLIENT] Result: CANCELED");

                break;

            case rclcpp_action::ResultCode::ABORTED:

                RCLCPP_ERROR(
                get_logger(),
                "[CLIENT] Result: ABORTED");

                break;

            default:

                RCLCPP_ERROR(
                get_logger(),
                "[CLIENT] Result: UNKNOWN");

                break;
            }
        };

        client_->async_send_goal(
        goal,
        options);
    }

    private:
    rclcpp_action::Client<Fibonacci>::SharedPtr
        client_;
};

int main(int argc,char ** argv){
  rclcpp::init(argc, argv);
  auto node =
    std::make_shared<AdvancedActionClient>();
  node->send_goal(15);
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}