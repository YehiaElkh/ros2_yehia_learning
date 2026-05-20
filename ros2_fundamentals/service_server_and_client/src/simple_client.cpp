#include "rclcpp/rclcpp.hpp"
#include "custom_msg_and_srv/srv/add_two_ints.hpp"

class SimpleClient : public rclcpp::Node
{
public:
    SimpleClient() : Node("simple_client")
    {

        // CRÉER LE CLIENT
        client_ = this->create_client<custom_msg_and_srv::srv::AddTwoInts>(
            "add_two_ints"   // même nom que le serveur !
        );

        // ATTENDRE QUE LE SERVEUR SOIT DISPONIBLE
        while (!client_->wait_for_service(std::chrono::seconds(1)))
        //              | attendre 1 seconde à chaque tentative
        {
            RCLCPP_WARN(this->get_logger(), "Waiting for server...");
            // RCLCPP_WARN : afficher un message de niveau WARNING (orange)
        }

        RCLCPP_INFO(this->get_logger(), "Server found ! Sending request...");

        // CRÉER ET ENVOYER LA REQUÊTE
        auto request = std::make_shared<custom_msg_and_srv::srv::AddTwoInts::Request>();
        request->a = 10;
        request->b = 25;

        // Envoyer la requête de façon ASYNCHRONE
        // le programme ne se bloque pas, il continue
        auto future = client_->async_send_request(
            request,
            std::bind(&SimpleClient::responseCallback, this, std::placeholders::_1)
        );
    }

private:
    
    // CALLBACK DE LA RÉPONSE
    // appelé automatiquement quand le serveur répond
    void responseCallback(
        rclcpp::Client<custom_msg_and_srv::srv::AddTwoInts>::SharedFuture future)
    {
        auto response = future.get();
        // future.get() : récupérer le résultat de la réponse

        RCLCPP_INFO(this->get_logger(), "Response received: sum = %ld", response->sum);
    }

    rclcpp::Client<custom_msg_and_srv::srv::AddTwoInts>::SharedPtr client_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SimpleClient>());
    rclcpp::shutdown();
    return 0;
}