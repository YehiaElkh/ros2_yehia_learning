#include <atomic>
#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "example_interfaces/action/fibonacci.hpp"

using namespace std::chrono_literals;

class SchedulerServer : public rclcpp::Node{
    public:

    using Action = example_interfaces::action::Fibonacci;

    using GoalHandle =
        rclcpp_action::ServerGoalHandle<Action>;

    struct GoalInfo{
        std::shared_ptr<GoalHandle> handle;

        int priority;

        uint64_t sequence;
    };

    struct GoalComparator{
        bool operator()(
        const GoalInfo & a,
        const GoalInfo & b) const
        {
        if (a.priority != b.priority)
        {
            return a.priority < b.priority;
        }

        return a.sequence > b.sequence;
        }
    };

    SchedulerServer()
    : Node("scheduler_server"),
        sequence_counter_(0)
    {
        action_server_ =
        rclcpp_action::create_server<Action>(
            this,
            "scheduled_fibonacci",

            std::bind(
            &SchedulerServer::handle_goal,
            this,
            std::placeholders::_1,
            std::placeholders::_2),

            std::bind(
            &SchedulerServer::handle_cancel,
            this,
            std::placeholders::_1),

            std::bind(
            &SchedulerServer::handle_accepted,
            this,
            std::placeholders::_1)
        );

        RCLCPP_INFO(
        get_logger(),
        "======================================");

        RCLCPP_INFO(
        get_logger(),
        " Scheduler Action Server started");

        RCLCPP_INFO(
        get_logger(),
        " Action: /scheduled_fibonacci");

        RCLCPP_INFO(
        get_logger(),
        " Priority scheduling enabled");

        RCLCPP_INFO(
        get_logger(),
        " ======================================");
    }

    private:

    rclcpp_action::GoalResponse handle_goal(
        const rclcpp_action::GoalUUID &,
        std::shared_ptr<const Action::Goal> goal)
    {
        RCLCPP_INFO(
        get_logger(),
        "[GOAL] Received order=%d",
        goal->order);

        if (goal->order <= 0){
        RCLCPP_WARN(
            get_logger(),
            "[GOAL] Rejected");

        return rclcpp_action::GoalResponse::REJECT;
        }

        if (goal->order > 100){
        RCLCPP_WARN(
            get_logger(),
            "[GOAL] Rejected: order too large");
        return rclcpp_action::GoalResponse::REJECT;
        }

        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }


    rclcpp_action::CancelResponse handle_cancel(
        const std::shared_ptr<GoalHandle> goal_handle)
    {
        (void)goal_handle;

        RCLCPP_WARN(
        get_logger(),
        "[CANCEL] Cancel request received");

        return rclcpp_action::CancelResponse::ACCEPT;
    }

    void handle_accepted(
        const std::shared_ptr<GoalHandle> goal_handle){

        const int order =
        goal_handle->get_goal()->order;

        const int priority =
        calculate_priority(order);

        GoalInfo info;

        info.handle = goal_handle;

        info.priority = priority;

        info.sequence =
        sequence_counter_.fetch_add(1);

        {
        std::lock_guard<std::mutex> lock(
            queue_mutex_);

        goal_queue_.push(info);
        }

        RCLCPP_INFO(
        get_logger(),
        "[SCHEDULER] Goal queued: order=%d priority=%d sequence=%lu",
        order,
        priority,
        info.sequence);

        scheduler_cv_.notify_one();
    }

    int calculate_priority(int order){
        if (order <= 10){
        return 1;
        }
        if (order <= 20){
        return 2;
        }
        if (order <= 50){
        return 3;
        }
        return 4;
    }

    void scheduler_loop(){
        while (rclcpp::ok()){
        GoalInfo next_goal;


        {
            std::unique_lock<std::mutex> lock(
            queue_mutex_);

            scheduler_cv_.wait(
            lock,
            [this](){
                return !goal_queue_.empty()
                || !rclcpp::ok();
            });

            if (!rclcpp::ok()){
            return;
            }

            next_goal =
            goal_queue_.top();

            goal_queue_.pop();
        }

        execute_goal(
            next_goal);
        }
    }

    void execute_goal(const GoalInfo & info){
        auto goal_handle =
        info.handle;

        const auto goal =
        goal_handle->get_goal();

        RCLCPP_INFO(
        get_logger(),
        "[EXECUTE] Starting order=%d priority=%d sequence=%lu",
        goal->order,
        info.priority,
        info.sequence);

        auto feedback = std::make_shared<Action::Feedback>();

        auto result = std::make_shared<Action::Result>();

        feedback->sequence.push_back(0);

        if (goal->order > 1){
        feedback->sequence.push_back(1);
        }

        for (int i = 2;  i < goal->order; ++i){
        if (goal_handle->is_canceling()){
            result->sequence =
            feedback->sequence;

            goal_handle->canceled(
            result);

            RCLCPP_WARN(
            get_logger(),
            "[EXECUTE] Goal canceled");

            return;
        }

        const auto next =
            feedback->sequence[i - 1] +
            feedback->sequence[i - 2];

        feedback->sequence.push_back(
            next);

        goal_handle->publish_feedback(
            feedback);

        RCLCPP_INFO(
            get_logger(),
            "[EXECUTE] order=%d progress=%zu",
            goal->order,
            feedback->sequence.size());

        std::this_thread::sleep_for(
            500ms);
        }

        result->sequence =
        feedback->sequence;

        goal_handle->succeed(
        result);

        RCLCPP_INFO(
        get_logger(),
        "[EXECUTE] Goal succeeded");
    }

    rclcpp_action::Server<Action>::SharedPtr
        action_server_;

    std::priority_queue<
        GoalInfo,
        std::vector<GoalInfo>,
        GoalComparator
    > goal_queue_;

    std::mutex queue_mutex_;

    std::condition_variable
        scheduler_cv_;

    std::atomic<uint64_t>
        sequence_counter_;

    std::thread scheduler_thread_{
        &SchedulerServer::scheduler_loop,
        this
    };
};

int main(int argc,char ** argv){
  rclcpp::init(argc, argv);
  auto node = std::make_shared<SchedulerServer>();
  rclcpp::executors::MultiThreadedExecutor
    executor(
      rclcpp::ExecutorOptions(),
      2);

  executor.add_node(node);
  executor.spin();
  rclcpp::shutdown();
  return 0;
}