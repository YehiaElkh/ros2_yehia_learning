#include <chrono>
#include <memory>
#include <string>
#include <vector>
#include "rclcpp/rclcpp.hpp"
#include "lifecycle_msgs/msg/state.hpp"
#include "lifecycle_msgs/msg/transition.hpp"
#include "lifecycle_msgs/srv/change_state.hpp"
#include "lifecycle_msgs/srv/get_state.hpp"

using namespace std::chrono_literals;

class LifecycleManager : public rclcpp::Node{
    public:

    LifecycleManager()
    : Node("lifecycle_manager"),
        startup_done_(false)
    {
        managed_nodes_ =
        {
        "/perception_lifecycle_node",
        "/planning_lifecycle_node",
        "/control_lifecycle_node"
        };

        RCLCPP_INFO(
        get_logger(),
        "Lifecycle Manager started");

        startup_timer_ =
        create_wall_timer(
            2s,
            std::bind(
            &LifecycleManager::startup,
            this));
    }


    private:

    void startup(){
        if (startup_done_){
        return;
        }

        startup_done_ = true;

        RCLCPP_INFO(
        get_logger(),
        "========================================");

        RCLCPP_INFO(
        get_logger(),
        "Starting managed system");

        RCLCPP_INFO(
        get_logger(),
        "========================================");

        for (const auto & node_name : managed_nodes_){
        RCLCPP_INFO(
            get_logger(),
            "Processing node: %s",
            node_name.c_str());


        if (!configure_node(node_name)){
            RCLCPP_ERROR(
            get_logger(),
            "Startup FAILED during configuration of %s",
            node_name.c_str());

            return;
        }

        if (!activate_node(node_name)){
            RCLCPP_ERROR(
            get_logger(),
            "Startup FAILED during activation of %s",
            node_name.c_str());

            return;
        }

        RCLCPP_INFO(
            get_logger(),
            "%s is ACTIVE",
            node_name.c_str());
        }

        RCLCPP_INFO(
        get_logger(),
        "========================================");

        RCLCPP_INFO(
        get_logger(),
        "All managed nodes are ACTIVE");

        RCLCPP_INFO(
        get_logger(),
        "System startup completed successfully");

        RCLCPP_INFO(
        get_logger(),
        "========================================");
    }

    bool configure_node(
        const std::string & node_name)
    {
        RCLCPP_INFO(
        get_logger(),
        "Configuring %s...",
        node_name.c_str());

        auto client =
        create_client<
            lifecycle_msgs::srv::ChangeState>(
            node_name + "/change_state");

        if (!client->wait_for_service(2s)){
        RCLCPP_ERROR(
            get_logger(),
            "ChangeState service unavailable for %s",
            node_name.c_str());

        return false;
        }

        auto request =
        std::make_shared<
            lifecycle_msgs::srv::ChangeState::Request>();

        request->transition.id =
        lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE;

        auto future = client->async_send_request(request);

        if (
        rclcpp::spin_until_future_complete(
            get_node_base_interface(),
            future)
        != rclcpp::FutureReturnCode::SUCCESS)
        {
        RCLCPP_ERROR(
            get_logger(),
            "Configure request failed for %s",
            node_name.c_str());

        return false;
        }

        if (!future.get()->success){
        RCLCPP_ERROR(
            get_logger(),
            "Configure transition rejected by %s",
            node_name.c_str());

        return false;
        }

        RCLCPP_INFO(
        get_logger(),
        "%s configure transition succeeded",
        node_name.c_str());

        return verify_state(
        node_name,
        lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE);
    }

    bool activate_node(const std::string & node_name){
        RCLCPP_INFO(
        get_logger(),
        "Activating %s...",
        node_name.c_str());

        auto client =
        create_client<
            lifecycle_msgs::srv::ChangeState>(
            node_name + "/change_state");

        if (!client->wait_for_service(2s)){
        RCLCPP_ERROR(
            get_logger(),
            "ChangeState service unavailable for %s",
            node_name.c_str());

        return false;
        }

        auto request =
        std::make_shared<
            lifecycle_msgs::srv::ChangeState::Request>();

        request->transition.id =
        lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE;

        auto future =
        client->async_send_request(request);

        if (
        rclcpp::spin_until_future_complete(
            get_node_base_interface(),
            future)
        != rclcpp::FutureReturnCode::SUCCESS)
        {
        RCLCPP_ERROR(
            get_logger(),
            "Activate request failed for %s",
            node_name.c_str());

        return false;
        }

        if (!future.get()->success){
        RCLCPP_ERROR(
            get_logger(),
            "Activate transition rejected by %s",
            node_name.c_str());

        return false;
        }

        RCLCPP_INFO(
        get_logger(),
        "%s activate transition succeeded",
        node_name.c_str());

        return verify_state(
        node_name,
        lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE);
    }

    bool verify_state(
        const std::string & node_name,
        uint8_t expected_state)
    {
        RCLCPP_INFO(
        get_logger(),
        "Verifying state of %s...",
        node_name.c_str());

        auto client =
        create_client<
            lifecycle_msgs::srv::GetState>(
            node_name + "/get_state");

        if (!client->wait_for_service(2s)){
        RCLCPP_ERROR(
            get_logger(),
            "GetState service unavailable for %s",
            node_name.c_str());

        return false;
        }

        auto request =
        std::make_shared<
            lifecycle_msgs::srv::GetState::Request>();

        auto future = client->async_send_request(request);

        if (
        rclcpp::spin_until_future_complete(
            get_node_base_interface(),
            future)
        != rclcpp::FutureReturnCode::SUCCESS)
        {
        RCLCPP_ERROR(
            get_logger(),
            "GetState request failed for %s",
            node_name.c_str());

        return false;
        }

        auto response = future.get();

        const auto actual_state =
        response->current_state.id;

        const auto actual_label =
        response->current_state.label;

        RCLCPP_INFO(
        get_logger(),
        "%s actual state: %s [%d]",
        node_name.c_str(),
        actual_label.c_str(),
        actual_state);

        if (actual_state != expected_state){
        RCLCPP_ERROR(
            get_logger(),
            "STATE VERIFICATION FAILED for %s",
            node_name.c_str());

        RCLCPP_ERROR(
            get_logger(),
            "Expected state ID: %d",
            expected_state);

        RCLCPP_ERROR(
            get_logger(),
            "Actual state ID: %d",
            actual_state);

        return false;
        }

        RCLCPP_INFO(
        get_logger(),
        "State verification SUCCESS for %s",
        node_name.c_str());

        return true;
    }

    std::vector<std::string> managed_nodes_;
    rclcpp::TimerBase::SharedPtr startup_timer_;
    bool startup_done_;
};

int main(int argc,char ** argv){
  rclcpp::init(argc, argv);
  auto manager = std::make_shared<LifecycleManager>();
  rclcpp::spin(manager);
  rclcpp::shutdown();
  return 0;
}