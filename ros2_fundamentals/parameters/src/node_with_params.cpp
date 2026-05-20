#include "rclcpp/rclcpp.hpp"

class NodeWithParams : public rclcpp::Node
{
public:
    NodeWithParams() : Node("node_with_params")
    {
        
        // DÉCLARER LES PARAMÈTRES
        // declare_parameter(nom, valeur_par_defaut)
        // Si le paramètre n'est pas fourni au démarrage,
        // la valeur par défaut est utilisée
        this->declare_parameter("timer_period", 1.0);
        //                       |               |
        //                      nom          valeur par défaut (double)

        this->declare_parameter("message", std::string("Hello ROS2 !"));
        //                                  |
        //                     std::string obligatoire pour les strings en C++

        this->declare_parameter("max_count", 10);
        //                                    |
        //                               valeur par défaut (int)


        // LIRE LES PARAMÈTRES
        double timer_period = this->get_parameter("timer_period").as_double();
        //                                                          |
        //                                         .as_double() : convertir en double
        //                                         .as_string() : convertir en string
        //                                         .as_int()    : convertir en int
        //                                         .as_bool()   : convertir en bool

        message_  = this->get_parameter("message").as_string();
        max_count_ = this->get_parameter("max_count").as_int();
        count_ = 0;

        RCLCPP_INFO(this->get_logger(), "Parameters loaded :");
        RCLCPP_INFO(this->get_logger(), "  timer_period : %.2f", timer_period);
        RCLCPP_INFO(this->get_logger(), "  message      : %s",   message_.c_str());
        RCLCPP_INFO(this->get_logger(), "  max_count    : %d",   max_count_);


        // CRÉER LE TIMER avec la période du paramètre
        timer_ = this->create_wall_timer(
            std::chrono::duration<double>(timer_period),
            //  | utiliser la valeur du paramètre
            std::bind(&NodeWithParams::timerCallback, this)
        );
    }

private:
    void timerCallback()
    {
        if (count_ >= max_count_)
        {
            RCLCPP_WARN(this->get_logger(), "Max count reached ! Stopping...");
            timer_->cancel();   // arrêter le timer
            return;
        }

        count_++;
        RCLCPP_INFO(this->get_logger(), "[%d/%d] %s",
            count_, max_count_, message_.c_str());
    }

    rclcpp::TimerBase::SharedPtr timer_;
    std::string message_;
    int max_count_;
    int count_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<NodeWithParams>());
    rclcpp::shutdown();
    return 0;
}