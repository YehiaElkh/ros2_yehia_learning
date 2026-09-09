#include <chrono>
#include <memory>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "std_msgs/msg/string.hpp"

using CallbackReturn =
    rclcpp_lifecycle::node_interfaces::
    LifecycleNodeInterface::CallbackReturn;

using namespace std::chrono_literals;


class PerceptionLifecycleNode : public rclcpp_lifecycle::LifecycleNode{
    public:

        PerceptionLifecycleNode()
            : LifecycleNode("perception_lifecycle_node"){
            RCLCPP_INFO(
                this->get_logger(),
                "Lifecycle node created"
            );
        }


    protected:
        CallbackReturn on_configure(
            const rclcpp_lifecycle::State &)
            override
            {
            RCLCPP_INFO(
                this->get_logger(),
                "Configuring node..."
            );

            RCLCPP_INFO(
                this->get_logger(),
                "Initializing perception resources..."
            );

            status_publisher_ =
                this->create_publisher<std_msgs::msg::String>(
                    "status",
                    10
                );

            RCLCPP_INFO(
                this->get_logger(),
                "Resources initialized"
            );

            return CallbackReturn::SUCCESS;
        }

        CallbackReturn on_activate(
            const rclcpp_lifecycle::State &)
            override
            {
            RCLCPP_INFO(
                this->get_logger(),
                "Activating node..."
            );

            status_publisher_->on_activate();

            timer_ =
                this->create_wall_timer(
                    1s,
                    std::bind(
                        &PerceptionLifecycleNode::process,
                        this
                    )
                );

            RCLCPP_INFO(
                this->get_logger(),
                "Perception node is ACTIVE"
            );

            return CallbackReturn::SUCCESS;
        }

        CallbackReturn on_deactivate(
            const rclcpp_lifecycle::State &)
            override
            {
            RCLCPP_INFO(
                this->get_logger(),
                "Deactivating node..."
            );

            if (timer_){
                timer_->cancel();
            }

            status_publisher_->on_deactivate();

            RCLCPP_INFO(
                this->get_logger(),
                "Perception node is INACTIVE"
            );

            return CallbackReturn::SUCCESS;
        }

        CallbackReturn on_cleanup(
            const rclcpp_lifecycle::State &)
            override
            {
            RCLCPP_INFO(
                this->get_logger(),
                "Cleaning up node..."
            );

            timer_.reset();

            status_publisher_.reset();

            RCLCPP_INFO(
                this->get_logger(),
                "Resources released"
            );

            return CallbackReturn::SUCCESS;
        }

        CallbackReturn on_shutdown(
            const rclcpp_lifecycle::State & state)
            override
        {
            RCLCPP_INFO(
                this->get_logger(),
                "Shutting down from state: %s",
                state.label().c_str()
            );

            timer_.reset();
            status_publisher_.reset();

            RCLCPP_INFO(
                this->get_logger(),
                "Shutdown complete"
            );

            return CallbackReturn::SUCCESS;
        }

    private:

        void process(){
            if (!status_publisher_){
                return;
            }

            if (!status_publisher_->is_activated()){
                return;
            }

            std_msgs::msg::String message;
            message.data = "Perception node is processing";
            status_publisher_->publish(message);
            RCLCPP_INFO(
                this->get_logger(),
                "%s",
                message.data.c_str()
            );
        }
        rclcpp_lifecycle::LifecyclePublisher<
            std_msgs::msg::String>::SharedPtr
            status_publisher_;

        rclcpp::TimerBase::SharedPtr timer_;
};


int main(int argc, char * argv[]){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<PerceptionLifecycleNode>();
    rclcpp::spin(node->get_node_base_interface());
    rclcpp::shutdown();
    return 0;
}