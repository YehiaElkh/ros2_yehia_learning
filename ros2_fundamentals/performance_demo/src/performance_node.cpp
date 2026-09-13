#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class PerformanceNode : public rclcpp::Node{
    public:
    PerformanceNode()
    : Node("performance_node")
    {
        fast_timer_ = this->create_wall_timer(
        100ms,
        std::bind(
            &PerformanceNode::fastCallback,
            this
        )
        );

        slow_timer_ = this->create_wall_timer(
        1000ms,
        std::bind(
            &PerformanceNode::slowCallback,
            this
        )
        );

        RCLCPP_INFO(
        this->get_logger(),
        "PerformanceNode started"
        );
    }

    private:
    void fastCallback(){
        const auto start =
        std::chrono::steady_clock::now();

        volatile std::uint64_t result = 0;

        for (std::uint64_t i = 0; i < 1000000; ++i){
        result += i;
        }

        const auto end =
        std::chrono::steady_clock::now();

        const auto execution_time =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - start);

        RCLCPP_INFO(
        this->get_logger(),
        "Fast callback: %ld us",
        execution_time.count()
        );
  }

  void slowCallback(){
    const auto start =
      std::chrono::steady_clock::now();

    std::this_thread::sleep_for(50ms);

    const auto end =
      std::chrono::steady_clock::now();

    const auto execution_time =
      std::chrono::duration_cast<
        std::chrono::milliseconds
      >(end - start);

    RCLCPP_INFO(
      this->get_logger(),
      "Slow callback: %ld ms",
      execution_time.count()
    );
  }

  rclcpp::TimerBase::SharedPtr fast_timer_;
  rclcpp::TimerBase::SharedPtr slow_timer_;
};

int main(int argc, char * argv[]){
  rclcpp::init(argc, argv);
  auto node =
    std::make_shared<PerformanceNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}