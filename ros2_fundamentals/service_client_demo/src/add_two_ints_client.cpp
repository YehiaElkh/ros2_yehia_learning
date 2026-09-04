#include <chrono>
#include <cstdlib>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

using namespace std::chrono_literals;

class AddTwoIntsClient : public rclcpp::Node{
    public:
        AddTwoIntsClient() : Node("add_two_ints_client"){
            client_ =
                this->create_client<
                    example_interfaces::srv::AddTwoInts
                >("/add_two_ints");
        }
        bool wait_for_service(){
            while (!client_->wait_for_service(1s)){
                if (!rclcpp::ok()){
                    return false;
                }
                RCLCPP_INFO(
                    this->get_logger(),
                    "Waiting for service..."
                );
            }
            return true;
        }
        void send_request(int64_t a, int64_t b){
            auto request =
                std::make_shared<
                    example_interfaces::srv::AddTwoInts::Request
                >();
            request->a = a;
            request->b = b;
            auto future =
                client_->async_send_request(request);
            auto result =
                rclcpp::spin_until_future_complete(
                    this->get_node_base_interface(),
                    future
                );

            if (result == rclcpp::FutureReturnCode::SUCCESS){
                RCLCPP_INFO(
                    this->get_logger(),
                    "Result: %ld + %ld = %ld",
                    a,
                    b,
                    future.get()->sum
                );
            }else{
                RCLCPP_ERROR(
                    this->get_logger(),
                    "Service call failed."
                );
            }
        }

    private:
        rclcpp::Client<
            example_interfaces::srv::AddTwoInts
        >::SharedPtr client_;
};

int main(int argc, char * argv[]){
    rclcpp::init(argc, argv);
    auto node =
        std::make_shared<AddTwoIntsClient>();
    if (!node->wait_for_service()){
        rclcpp::shutdown();
        return 1;
    }
    node->send_request(10, 20);
    rclcpp::shutdown();
    return 0;
}