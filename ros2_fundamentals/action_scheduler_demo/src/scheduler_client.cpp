#include <chrono>
#include <functional>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "example_interfaces/action/fibonacci.hpp"

using namespace std::chrono_literals;

class SchedulerClient : public rclcpp::Node{
    public:

    using Action =
        example_interfaces::action::Fibonacci;

    using GoalHandle =
        rclcpp_action::ClientGoalHandle<Action>;

    SchedulerClient()
    : Node("scheduler_client"){
        client_ =
        rclcpp_action::create_client<Action>(
            this,
            "scheduled_fibonacci");
    }

    void send_goal(int order){
        if (!client_->wait_for_action_server(5s)){
        RCLCPP_ERROR(
            get_logger(),
            "Action server not available");

        return;
        }

        Action::Goal goal;
        goal.order = order;

        RCLCPP_INFO(
        get_logger(),
        "[CLIENT] Sending order=%d",
        order);

        rclcpp_action::Client<Action>::SendGoalOptions
        options;

        options.feedback_callback =
        [this](
            GoalHandle::SharedPtr,
            const std::shared_ptr<const Action::Feedback>
            feedback)
        {
            RCLCPP_INFO(
            get_logger(),
            "[CLIENT] Feedback size=%zu",
            feedback->sequence.size());
        };

        options.result_callback =
        [this](
            const GoalHandle::WrappedResult & result){
            switch (result.code){
            case rclcpp_action::ResultCode::SUCCEEDED:

                RCLCPP_INFO(
                get_logger(),
                "[CLIENT] SUCCEEDED");

                break;

            case rclcpp_action::ResultCode::CANCELED:

                RCLCPP_WARN(
                get_logger(),
                "[CLIENT] CANCELED");

                break;

            case rclcpp_action::ResultCode::ABORTED:

                RCLCPP_ERROR(
                get_logger(),
                "[CLIENT] ABORTED");

                break;

            default:

                RCLCPP_ERROR(
                get_logger(),
                "[CLIENT] UNKNOWN");

                break;
            }
        };

        client_->async_send_goal(
        goal,
        options);
    }


    private:
    rclcpp_action::Client<Action>::SharedPtr
        client_;
};

int main(int argc, char ** argv){
  rclcpp::init(argc, argv);
  auto node = std::make_shared<SchedulerClient>();
  node->send_goal(40);
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}