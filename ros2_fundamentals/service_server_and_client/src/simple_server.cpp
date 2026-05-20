#include "rclcpp/rclcpp.hpp"
#include "custom_msg_and_srv/srv/add_two_ints.hpp"
//                                |
//          ROS2 convertit AddTwoInts.srv → add_two_ints.hpp (snake_case)

class SimpleServer : public rclcpp::Node
{
public:
    SimpleServer() : Node("simple_server")
    {
        
        // CRÉER LE SERVICE
        server_ = this->create_service<custom_msg_and_srv::srv::AddTwoInts>(
        //                              |
        //                    type du service
            "add_two_ints",   // nom du service
            std::bind(&SimpleServer::serviceCallback, this,
                std::placeholders::_1,   // request
                std::placeholders::_2)   // response
            // 2 placeholders car le callback prend 2 arguments
        );

        RCLCPP_INFO(this->get_logger(), "Server started, waiting for requests...");
    }

private:
    // --------------------------------------------------------
    // CALLBACK DU SERVICE
    // appelé automatiquement quand un client envoie une requête
    // --------------------------------------------------------
    void serviceCallback(
        const custom_msg_and_srv::srv::AddTwoInts::Request::SharedPtr request,
        //    | ce que le client nous envoie (a et b)
        custom_msg_and_srv::srv::AddTwoInts::Response::SharedPtr response)
        //    | ce qu'on va retourner au client (sum)
    {
        // Calculer la somme
        response->sum = request->a + request->b;

        RCLCPP_INFO(this->get_logger(),
            "Request received: a=%ld, b=%ld → sum=%ld",
            request->a, request->b, response->sum);
        // %ld = format pour int64
    }

    rclcpp::Service<custom_msg_and_srv::srv::AddTwoInts>::SharedPtr server_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SimpleServer>());
    rclcpp::shutdown();
    return 0;
}