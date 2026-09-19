#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"

using CallbackReturn =
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;


class PerceptionLifecycleNode
  : public rclcpp_lifecycle::LifecycleNode
{
    public:

    PerceptionLifecycleNode()
    : LifecycleNode("perception_lifecycle_node")
    {
        RCLCPP_INFO(
        get_logger(),
        "Perception Lifecycle Node created");
    }


    protected:

    CallbackReturn on_configure(
        const rclcpp_lifecycle::State &)
    {
        RCLCPP_INFO(
        get_logger(),
        "Perception: configuring");

        RCLCPP_INFO(
        get_logger(),
        "Perception: loading model");

        RCLCPP_INFO(
        get_logger(),
        "Perception: initializing publishers/subscribers");

        RCLCPP_INFO(
        get_logger(),
        "Perception: configuration complete");

        return CallbackReturn::SUCCESS;
    }


    CallbackReturn on_activate(
        const rclcpp_lifecycle::State & state)
    {
        RCLCPP_INFO(
        get_logger(),
        "Perception: activating");

        return LifecycleNode::on_activate(state);
    }


    CallbackReturn on_deactivate(
        const rclcpp_lifecycle::State & state)
    {
        RCLCPP_INFO(
        get_logger(),
        "Perception: deactivating");

        return LifecycleNode::on_deactivate(state);
    }


    CallbackReturn on_cleanup(
        const rclcpp_lifecycle::State &)
    {
        RCLCPP_INFO(
        get_logger(),
        "Perception: cleaning up");

        return CallbackReturn::SUCCESS;
    }


    CallbackReturn on_shutdown(
        const rclcpp_lifecycle::State &)
    {
        RCLCPP_INFO(
        get_logger(),
        "Perception: shutting down");

        return CallbackReturn::SUCCESS;
    }
};

int main(int argc, char ** argv){
  rclcpp::init(argc, argv);
  auto node = std::make_shared<PerceptionLifecycleNode>();
  rclcpp::spin(node->get_node_base_interface());
  rclcpp::shutdown();
  return 0;
}