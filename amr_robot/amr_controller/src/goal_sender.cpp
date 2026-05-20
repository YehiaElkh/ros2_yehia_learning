#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
// rclcpp_action : bibliothèque pour utiliser les actions ROS2
// On utilise l'action navigate_to_pose de Nav2

#include "nav2_msgs/action/navigate_to_pose.hpp"
// nav2_msgs : messages Nav2
// NavigateToPose : action pour envoyer un goal de navigation

#include "geometry_msgs/msg/pose_stamped.hpp"
// PoseStamped : position + orientation + timestamp + frame

class GoalSender : public rclcpp::Node
{
public:
    // Alias pour simplifier l'écriture
    using NavigateToPose = nav2_msgs::action::NavigateToPose;
    using GoalHandle     = rclcpp_action::ClientGoalHandle<NavigateToPose>;

    GoalSender() : Node("goal_sender")
    {
        // --------------------------------------------------------
        // DÉCLARER LES PARAMÈTRES
        // --------------------------------------------------------
        // On peut changer le goal depuis le terminal sans recompiler
        this->declare_parameter("goal_x",   2.0);   // position X du goal
        this->declare_parameter("goal_y",   1.0);   // position Y du goal
        this->declare_parameter("goal_yaw", 0.0);   // orientation du goal

        // Lire les paramètres
        goal_x_   = this->get_parameter("goal_x").as_double();
        goal_y_   = this->get_parameter("goal_y").as_double();
        goal_yaw_ = this->get_parameter("goal_yaw").as_double();

        // --------------------------------------------------------
        // CRÉER LE CLIENT D'ACTION Nav2
        // --------------------------------------------------------
        client_ = rclcpp_action::create_client<NavigateToPose>(
            this,
            "navigate_to_pose"
            // Nom de l'action Nav2 — doit correspondre exactement
        );

        RCLCPP_INFO(this->get_logger(),
            "GoalSender ready — goal: (%.2f, %.2f, %.2f)",
            goal_x_, goal_y_, goal_yaw_);

        // --------------------------------------------------------
        // ATTENDRE QUE NAV2 SOIT PRÊT
        // --------------------------------------------------------
        // On utilise un timer pour attendre Nav2 sans bloquer
        timer_ = this->create_wall_timer(
            std::chrono::seconds(1),
            std::bind(&GoalSender::checkAndSendGoal, this)
            // Vérifier toutes les secondes si Nav2 est prêt
        );
    }

private:

    // --------------------------------------------------------
    // VÉRIFIER ET ENVOYER LE GOAL
    // --------------------------------------------------------
    void checkAndSendGoal()
    {
        // Vérifier si le serveur d'action Nav2 est disponible
        if (!client_->wait_for_action_server(std::chrono::seconds(0)))
        {
            RCLCPP_WARN(this->get_logger(), "Waiting for Nav2 action server...");
            return;
            // Revenir et réessayer dans 1 seconde
        }

        // Nav2 est prêt — annuler le timer
        timer_->cancel();

        RCLCPP_INFO(this->get_logger(), "Nav2 ready ! Sending goal...");

        // Envoyer le goal
        sendGoal(goal_x_, goal_y_, goal_yaw_);
    }

    // --------------------------------------------------------
    // CONSTRUIRE ET ENVOYER LE GOAL
    // --------------------------------------------------------
    void sendGoal(double x, double y, double yaw)
    {
        // --------------------------------------------------------
        // CONSTRUIRE LE MESSAGE GOAL
        // --------------------------------------------------------
        auto goal_msg = NavigateToPose::Goal();
        // NavigateToPose::Goal : structure contenant la destination

        // Frame de référence = map
        goal_msg.pose.header.frame_id = "map";
        // frame_id="map" : les coordonnées sont dans le repère map

        // Timestamp
        goal_msg.pose.header.stamp = this->get_clock()->now();

        // Position XYZ
        goal_msg.pose.pose.position.x = x;
        goal_msg.pose.pose.position.y = y;
        goal_msg.pose.pose.position.z = 0.0;
        // z=0.0 : robot au sol

        // --------------------------------------------------------
        // CONVERTIR YAW EN QUATERNION
        // --------------------------------------------------------
        // Nav2 utilise des quaternions pour l'orientation
        // Pour une rotation autour de Z (yaw) :
        // w = cos(yaw/2), z = sin(yaw/2), x = y = 0
        goal_msg.pose.pose.orientation.x = 0.0;
        goal_msg.pose.pose.orientation.y = 0.0;
        goal_msg.pose.pose.orientation.z = std::sin(yaw / 2.0);
        goal_msg.pose.pose.orientation.w = std::cos(yaw / 2.0);

        // --------------------------------------------------------
        // DÉFINIR LES CALLBACKS
        // --------------------------------------------------------
        auto options = rclcpp_action::Client<NavigateToPose>::SendGoalOptions();

        // Callback 1 : réponse du serveur (accepté/rejeté)
        options.goal_response_callback =
            std::bind(&GoalSender::goalResponseCallback, this,
                std::placeholders::_1);

        // Callback 2 : feedback pendant la navigation
        options.feedback_callback =
            std::bind(&GoalSender::feedbackCallback, this,
                std::placeholders::_1, std::placeholders::_2);

        // Callback 3 : résultat final
        options.result_callback =
            std::bind(&GoalSender::resultCallback, this,
                std::placeholders::_1);

        // Envoyer le goal de façon asynchrone
        client_->async_send_goal(goal_msg, options);

        RCLCPP_INFO(this->get_logger(),
            "Goal sent: x=%.2f, y=%.2f, yaw=%.2f", x, y, yaw);
    }

    // --------------------------------------------------------
    // CALLBACK 1 : réponse du serveur Nav2
    // --------------------------------------------------------
    void goalResponseCallback(const GoalHandle::SharedPtr & goal_handle)
    {
        if (!goal_handle)
        {
            // Nav2 a rejeté le goal (obstacle, goal hors carte...)
            RCLCPP_ERROR(this->get_logger(), "Goal rejected by Nav2 !");
        }
        else
        {
            RCLCPP_INFO(this->get_logger(), "Goal accepted ! Robot is navigating...");
        }
    }

    // --------------------------------------------------------
    // CALLBACK 2 : feedback pendant la navigation
    // --------------------------------------------------------
    void feedbackCallback(
    GoalHandle::SharedPtr,
    const std::shared_ptr<const NavigateToPose::Feedback> feedback){
        
        RCLCPP_INFO_THROTTLE(
            this->get_logger(),
            *this->get_clock(),
            2000,
            // Afficher toutes les 2000ms = 2 secondes
            "Distance remaining: %.2fm",
            feedback->distance_remaining
        );
    }

    // --------------------------------------------------------
    // CALLBACK 3 : résultat final
    // --------------------------------------------------------
    void resultCallback(const GoalHandle::WrappedResult & result)
    {
        switch (result.code)
        {
            case rclcpp_action::ResultCode::SUCCEEDED:
                RCLCPP_INFO(this->get_logger(),
                    "✅ Goal reached successfully !");
                break;

            case rclcpp_action::ResultCode::ABORTED:
                RCLCPP_ERROR(this->get_logger(),
                    "❌ Goal aborted — Nav2 could not reach the goal !");
                break;

            case rclcpp_action::ResultCode::CANCELED:
                RCLCPP_WARN(this->get_logger(),
                    "⚠️  Goal canceled !");
                break;

            default:
                RCLCPP_ERROR(this->get_logger(), "Unknown result code !");
                break;
        }
    }

    // --------------------------------------------------------
    // ATTRIBUTS PRIVÉS
    // --------------------------------------------------------
    rclcpp_action::Client<NavigateToPose>::SharedPtr client_;
    // Client d'action pour communiquer avec Nav2

    rclcpp::TimerBase::SharedPtr timer_;
    // Timer pour attendre que Nav2 soit prêt

    double goal_x_, goal_y_, goal_yaw_;
    // Coordonnées du goal
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<GoalSender>());
    rclcpp::shutdown();
    return 0;
}
