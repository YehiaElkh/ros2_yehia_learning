#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

enum class NodeState{
  UNCONFIGURED,
  INACTIVE,
  ACTIVE,
  FAILED
};

struct ManagedNode{
  std::string name;
  NodeState state;
};

class SystemOrchestrator : public rclcpp::Node{
    public:

    SystemOrchestrator()
    : Node("system_orchestrator"),
        system_ready_(false)
    {
        managed_nodes_ = {
        {"camera", NodeState::UNCONFIGURED},
        {"detection", NodeState::UNCONFIGURED},
        {"tracking", NodeState::UNCONFIGURED},
        {"planning", NodeState::UNCONFIGURED},
        {"control", NodeState::UNCONFIGURED}
        };

        RCLCPP_INFO(
        get_logger(),
        "System Orchestrator created");
    }

    void startup(){
        RCLCPP_INFO(
        get_logger(),
        "========== SYSTEM STARTUP ==========");

        for (auto & node : managed_nodes_){
        if (!configure_node(node)){
            startup_failed(node.name);
            return;
        }

        if (!activate_node(node)){
            startup_failed(node.name);
            return;
        }
        }

        system_ready_ = true;

        RCLCPP_INFO(
        get_logger(),
        "=====================================");

        RCLCPP_INFO(
        get_logger(),
        "SYSTEM READY");
    }

    void shutdown_system(){
        RCLCPP_INFO(
        get_logger(),
        "========== SYSTEM SHUTDOWN ==========");

        for (auto it = managed_nodes_.rbegin();
            it != managed_nodes_.rend();
            ++it){
        deactivate_node(*it);
        }

        system_ready_ = false;

        RCLCPP_INFO(
        get_logger(),
        "SYSTEM SHUTDOWN COMPLETE");
    }

    private:
    bool configure_node(ManagedNode & node){
        RCLCPP_INFO(
        get_logger(),
        "Configuring: %s",
        node.name.c_str());

        std::this_thread::sleep_for(300ms);

        node.state = NodeState::INACTIVE;

        RCLCPP_INFO(
        get_logger(),
        "%s → INACTIVE",
        node.name.c_str());

        return true;
    }

    bool activate_node(ManagedNode & node){
        RCLCPP_INFO(
        get_logger(),
        "Activating: %s",
        node.name.c_str());

        std::this_thread::sleep_for(300ms);

        node.state = NodeState::ACTIVE;

        RCLCPP_INFO(
        get_logger(),
        "%s → ACTIVE",
        node.name.c_str());

        return true;
    }

    void deactivate_node(ManagedNode & node){
        if (node.state != NodeState::ACTIVE){
        return;
        }

        RCLCPP_INFO(
        get_logger(),
        "Deactivating: %s",
        node.name.c_str());

        std::this_thread::sleep_for(200ms);

        node.state = NodeState::INACTIVE;

        RCLCPP_INFO(
        get_logger(),
        "%s → INACTIVE",
        node.name.c_str());
    }

    void startup_failed(const std::string & node_name){
        RCLCPP_ERROR(
        get_logger(),
        "Startup failed at node: %s",
        node_name.c_str());

        RCLCPP_ERROR(
        get_logger(),
        "System will not enter READY state");

        system_ready_ = false;
    }

    std::vector<ManagedNode> managed_nodes_;
    bool system_ready_;
};

int main(int argc,char ** argv){
  rclcpp::init(argc, argv);
  auto orchestrator =
    std::make_shared<SystemOrchestrator>();
  orchestrator->startup();
  std::this_thread::sleep_for(2s);
  orchestrator->shutdown_system();
  rclcpp::shutdown();
  return 0;
}