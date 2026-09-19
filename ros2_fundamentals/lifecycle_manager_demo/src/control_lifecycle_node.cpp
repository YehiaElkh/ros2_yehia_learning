#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"

using CallbackReturn =
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;


class ControlLifecycleNode
  : public rclcpp_lifecycle::LifecycleNode
{
    public:

    ControlLifecycleNode()
    : LifecycleNode("control_lifecycle_node")
    {
        RCLCPP_INFO(
        get_logger(),
        "Control Lifecycle Node created");
    }


    protected:

    CallbackReturn on_configure(
        const rclcpp_lifecycle::State &)
    {
        RCLCPP_INFO(
        get_logger(),
        "Control: configuring");

        RCLCPP_INFO(
        get_logger(),
        "Control: initializing controller");

        RCLCPP_INFO(
        get_logger(),
        "Control: configuration complete");

        return CallbackReturn::SUCCESS;
    }

    CallbackReturn on_activate(
        const rclcpp_lifecycle::State & state)
    {
        RCLCPP_INFO(
        get_logger(),
        "Control: activating");

        return LifecycleNode::on_activate(state);
    }

    CallbackReturn on_deactivate(
        const rclcpp_lifecycle::State & state)
    {
        RCLCPP_INFO(
        get_logger(),
        "Control: deactivating");

        return LifecycleNode::on_deactivate(state);
    }

    CallbackReturn on_cleanup(
        const rclcpp_lifecycle::State &)
    {
        RCLCPP_INFO(
        get_logger(),
        "Control: cleaning up");

        return CallbackReturn::SUCCESS;
    }

    CallbackReturn on_shutdown(
        const rclcpp_lifecycle::State &)
    {
        RCLCPP_INFO(
        get_logger(),
        "Control: shutting down");

        return CallbackReturn::SUCCESS;
    }
};

int main(int argc, char ** argv){
  rclcpp::init(argc, argv);
  auto node = std::make_shared<ControlLifecycleNode>();
  rclcpp::spin(node->get_node_base_interface());
  rclcpp::shutdown();
  return 0;
}