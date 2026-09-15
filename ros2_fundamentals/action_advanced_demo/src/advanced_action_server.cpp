#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "example_interfaces/action/fibonacci.hpp"

using namespace std::chrono_literals;

class AdvancedActionServer : public rclcpp::Node{
    public:

    using Fibonacci = example_interfaces::action::Fibonacci;

    using GoalHandleFibonacci =
        rclcpp_action::ServerGoalHandle<Fibonacci>;

    AdvancedActionServer()
    : Node("advanced_action_server"){

        callback_group_ =
        create_callback_group(
            rclcpp::CallbackGroupType::Reentrant);

        action_server_ =
        rclcpp_action::create_server<Fibonacci>(
            this,

            "advanced_fibonacci",

            std::bind(
            &AdvancedActionServer::handle_goal,
            this,
            std::placeholders::_1,
            std::placeholders::_2),

            std::bind(
            &AdvancedActionServer::handle_cancel,
            this,
            std::placeholders::_1),

            std::bind(
            &AdvancedActionServer::handle_accepted,
            this,
            std::placeholders::_1),

            rcl_action_server_get_default_options()
        );


        RCLCPP_INFO(
        get_logger(),
        "======================================");

        RCLCPP_INFO(
        get_logger(),
        " Advanced Action Server started");

        RCLCPP_INFO(
        get_logger(),
        " Action: /advanced_fibonacci");

        RCLCPP_INFO(
        get_logger(),
        " Policy: Single active goal");

        RCLCPP_INFO(
        get_logger(),
        " Policy: New goal preempts current goal");

        RCLCPP_INFO(
        get_logger(),
        "======================================");
    }


    private:

    rclcpp_action::GoalResponse handle_goal(
        const rclcpp_action::GoalUUID & uuid,
        std::shared_ptr<const Fibonacci::Goal> goal)
    {
        (void)uuid;

        RCLCPP_INFO(
        get_logger(),
        "[GOAL] Received goal: order=%d",
        goal->order);

        if (goal->order <= 0){
        RCLCPP_WARN(
            get_logger(),
            "[GOAL] Rejected: order must be > 0");

        return rclcpp_action::GoalResponse::REJECT;
        }


        if (goal->order > 100){
        RCLCPP_WARN(
            get_logger(),
            "[GOAL] Rejected: order too large");

        return rclcpp_action::GoalResponse::REJECT;
        }

        {
        std::lock_guard<std::mutex> lock(goal_mutex_);

        if (active_goal_)
        {
            RCLCPP_INFO(
            get_logger(),
            "[GOAL] Active goal exists -> new goal will preempt it");
        }
        }


        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }

    rclcpp_action::CancelResponse handle_cancel(
        const std::shared_ptr<GoalHandleFibonacci> goal_handle)
    {
        (void)goal_handle;

        RCLCPP_WARN(
        get_logger(),
        "[CANCEL] Cancellation request received");


        return rclcpp_action::CancelResponse::ACCEPT;
    }

    void handle_accepted(
        const std::shared_ptr<GoalHandleFibonacci> goal_handle)
    {
        std::shared_ptr<GoalHandleFibonacci> previous_goal;
        {
        std::lock_guard<std::mutex> lock(goal_mutex_);

        previous_goal = active_goal_;

        active_goal_ = goal_handle;
        }

        if (previous_goal &&
            previous_goal != goal_handle)
        {
        RCLCPP_WARN(
            get_logger(),
            "[PREEMPT] Requesting cancellation of previous goal");

        }

        std::thread(
        std::bind(
            &AdvancedActionServer::execute,
            this,
            std::placeholders::_1),
        goal_handle
        ).detach();
    }



    void execute(
        const std::shared_ptr<GoalHandleFibonacci> goal_handle)
    {
        const auto goal =
        goal_handle->get_goal();

        auto feedback =
        std::make_shared<Fibonacci::Feedback>();

        auto result =
        std::make_shared<Fibonacci::Result>();

        feedback->sequence.clear();

        feedback->sequence.push_back(0);

        if (goal->order > 1){
        feedback->sequence.push_back(1);
        }

        RCLCPP_INFO(
        get_logger(),
        "[EXECUTE] Starting goal order=%d",
        goal->order);

        const auto start_time =
        std::chrono::steady_clock::now();

        for (
        int i = 2;
        i < goal->order;
        ++i){

        if (goal_handle->is_canceling()){
            result->sequence =
            feedback->sequence;


            RCLCPP_WARN(
            get_logger(),
            "[EXECUTE] Goal canceled");

            goal_handle->canceled(result);

            clear_active_goal(goal_handle);

            return;
        }

        const auto now =
            std::chrono::steady_clock::now();

        const auto elapsed =
            std::chrono::duration_cast<
            std::chrono::seconds>(
                now - start_time);

        if (elapsed.count() >= 10){
            result->sequence =
            feedback->sequence;


            RCLCPP_ERROR(
            get_logger(),
            "[EXECUTE] Goal aborted: timeout");


            goal_handle->abort(result);


            clear_active_goal(goal_handle);

            return;
        }

        const auto next =
            feedback->sequence[i - 1] +
            feedback->sequence[i - 2];

        feedback->sequence.push_back(next);

        goal_handle->publish_feedback(
            feedback);

        RCLCPP_INFO(
            get_logger(),
            "[EXECUTE] Progress: %zu values",
            feedback->sequence.size());

        std::this_thread::sleep_for(500ms);
        }


        result->sequence =
        feedback->sequence;

        if (goal_handle->is_canceling()){
        goal_handle->canceled(result);

        RCLCPP_WARN(
            get_logger(),
            "[EXECUTE] Goal canceled at completion");

        clear_active_goal(goal_handle);

        return;
        }

        goal_handle->succeed(result);

        RCLCPP_INFO(
        get_logger(),
        "[EXECUTE] Goal SUCCEEDED");


        clear_active_goal(goal_handle);
    }

    void clear_active_goal(
        const std::shared_ptr<GoalHandleFibonacci> & goal_handle){
        std::lock_guard<std::mutex> lock(goal_mutex_);

        if (active_goal_ == goal_handle){
        active_goal_.reset();

        RCLCPP_INFO(
            get_logger(),
            "[GOAL MANAGER] Active goal cleared");
        }
    }

    rclcpp_action::Server<Fibonacci>::SharedPtr
        action_server_;

    rclcpp::CallbackGroup::SharedPtr
        callback_group_;
    std::mutex goal_mutex_;
    std::shared_ptr<GoalHandleFibonacci>
        active_goal_;
};

int main(int argc,char ** argv){
  rclcpp::init(argc, argv);
  auto node = std::make_shared<AdvancedActionServer>();
  rclcpp::executors::MultiThreadedExecutor executor(
    rclcpp::ExecutorOptions(),
    2
  );
  executor.add_node(node);
  executor.spin();
  rclcpp::shutdown();
  return 0;
}