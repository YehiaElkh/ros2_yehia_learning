#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"

using CallbackReturn =
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;


class PlanningLifecycleNode
  : public rclcpp_lifecycle::LifecycleNode
{
    public:

    PlanningLifecycleNode()
    : LifecycleNode("planning_lifecycle_node")
    {
        RCLCPP_INFO(
        get_logger(),
        "Planning Lifecycle Node created");
    }


    protected:

    CallbackReturn on_configure(
        const rclcpp_lifecycle::State &)
    {
        RCLCPP_INFO(
        get_logger(),
        "Planning: configuring");

        RCLCPP_INFO(
        get_logger(),
        "Planning: initializing planner");

        RCLCPP_INFO(
        get_logger(),
        "Planning: configuration complete");

        return CallbackReturn::SUCCESS;
    }


    CallbackReturn on_activate(
        const rclcpp_lifecycle::State & state)
    {
        RCLCPP_INFO(
        get_logger(),
        "Planning: activating");

        return LifecycleNode::on_activate(state);
    }


    CallbackReturn on_deactivate(
        const rclcpp_lifecycle::State & state)
    {
        RCLCPP_INFO(
        get_logger(),
        "Planning: deactivating");

        return LifecycleNode::on_deactivate(state);
    }


    CallbackReturn on_cleanup(
        const rclcpp_lifecycle::State &)
    {
        RCLCPP_INFO(
        get_logger(),
        "Planning: cleaning up");

        return CallbackReturn::SUCCESS;
    }


    CallbackReturn on_shutdown(
        const rclcpp_lifecycle::State &)
    {
        RCLCPP_INFO(
        get_logger(),
        "Planning: shutting down");

        return CallbackReturn::SUCCESS;
    }
};

int main(int argc, char ** argv){
  rclcpp::init(argc, argv);
  auto node = std::make_shared<PlanningLifecycleNode>();
  rclcpp::spin(node->get_node_base_interface());
  rclcpp::shutdown();
  return 0;
}