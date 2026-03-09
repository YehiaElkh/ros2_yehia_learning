#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class SimpleSubscriber : public rclcpp::Node
{
public:
    SimpleSubscriber() : Node("simple_subscriber")
    {
        
        // CRÉER LE SUBSCRIBER
        
        subscriber_ = this->create_subscription<std_msgs::msg::String>(
        //                                        |
        //                           même type que le publisher !
            "chatter",   // même nom de topic que le publisher !
            10,          // QoS : taille de la file d'attente
            std::bind(&SimpleSubscriber::messageCallback, this, std::placeholders::_1)
            //                                                    |
            //                               _1 = placeholder pour le message reçu
            //                               car messageCallback prend 1 argument (le msg)
        );

        RCLCPP_INFO(this->get_logger(), "Subscriber started !");
    }

private:
    
    // CALLBACK DU SUBSCRIBER — exécuté à chaque message reçu
    void messageCallback(const std_msgs::msg::String::SharedPtr msg)
    //                   |                                |
    //               const : on ne modifie pas le message  SharedPtr : pointeur intelligent
    {
        RCLCPP_INFO(this->get_logger(), "Received: '%s'", msg->data.c_str());
        //                                                      |
        //                                        msg-> : accéder aux champs via pointeur
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriber_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SimpleSubscriber>());
    rclcpp::shutdown();
    return 0;
}