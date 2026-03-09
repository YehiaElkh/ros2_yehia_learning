// INCLUDE — importer les bibliothèques nécessaires
#include "rclcpp/rclcpp.hpp"          // bibliothèque principale ROS2 C++
#include "std_msgs/msg/string.hpp"     // type de message : une simple chaîne de texte


// CLASSE — notre nœud hérite de rclcpp::Node
// "public rclcpp::Node" signifie que notre classe EST un nœud ROS2
// Elle hérite de toutes les fonctionnalités d'un nœud
class SimplePublisher : public rclcpp::Node
{
public:
    // CONSTRUCTEUR — exécuté une seule fois au démarrage
    SimplePublisher() : Node("simple_publisher"), count_(0)
    //                   |                         |
    //         nom du nœud dans ROS2        initialisation de count_ à 0
    {
        // CRÉER LE PUBLISHER
        
        publisher_ = this->create_publisher<std_msgs::msg::String>(
        //                                  |
        //                    type du message qu'on va publier
            "chatter",   // nom du topic (l'adresse de la boîte aux lettres)
            10           // QoS (Quality of Service) : taille de la file d'attente
                         // 10 = garder les 10 derniers messages en mémoire
        );

        
        // CRÉER LE TIMER
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(500),           // période : toutes les 500ms
            std::bind(&SimplePublisher::timerCallback, this)
            // std::bind : "quand le timer sonne, appelle timerCallback de CETTE instance"
        );

        RCLCPP_INFO(this->get_logger(), "Publisher started !");
        // RCLCPP_INFO : afficher un message de niveau INFO dans le terminal
        // get_logger() : récupère le logger associé à ce nœud
    }

private:
    
    // CALLBACK DU TIMER — exécuté toutes les 500ms
    void timerCallback()
    {
        // Créer un message vide du bon type
        auto msg = std_msgs::msg::String();

        // Remplir le champ "data" du message
        msg.data = "Hello ROS2 n:" + std::to_string(count_++);
        //                                            |
        //                              count_++ : incrémenter après utilisation

        // Afficher ce qu'on publie
        RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", msg.data.c_str());
        //                                                           |
        //                                          .c_str() : convertir string en char*

        // Publier le message sur le topic
        publisher_->publish(msg);
    }

    
    // ATTRIBUTS PRIVÉS — variables membres de la classe
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    // SharedPtr = pointeur intelligent (gestion mémoire automatique)

    rclcpp::TimerBase::SharedPtr timer_;
    // Le timer qui déclenche timerCallback périodiquement

    int count_;
    // Compteur pour numéroter les messages
};


// MAIN — point d'entrée du programme
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    // init : initialiser ROS2, lire les arguments de ligne de commande

    rclcpp::spin(std::make_shared<SimplePublisher>());
    // make_shared : créer une instance de SimplePublisher
    // spin : boucle infinie — maintenir le nœud actif et traiter les événements
    //        (callbacks, messages, timers...)
    //        Le programme reste bloqué ici jusqu'à Ctrl+C

    rclcpp::shutdown();
    // Nettoyage propre de ROS2 avant de quitter
    return 0;
}