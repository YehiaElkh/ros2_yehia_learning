#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "nav2_msgs/action/navigate_to_pose.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"

// ============================================================
// WAYPOINT SENDER
// ============================================================
// Ce nœud envoie une séquence de waypoints au robot
// Le robot navigue vers chaque waypoint l'un après l'autre
// Utilise l'action navigate_to_pose pour chaque waypoint
// ============================================================

class WaypointSender : public rclcpp::Node
{
public:
    using NavigateToPose = nav2_msgs::action::NavigateToPose;
    using GoalHandle     = rclcpp_action::ClientGoalHandle<NavigateToPose>;

    WaypointSender() : Node("waypoint_sender"), current_waypoint_(0)
    // current_waypoint_ : index du waypoint actuel dans la liste
    {
        // --------------------------------------------------------
        // DÉFINIR LES WAYPOINTS
        // --------------------------------------------------------
        // Chaque waypoint = {x, y, yaw}
        // Le robot visitera ces points dans l'ordre
        waypoints_ = {
            {2.0,  1.0, 0.0},    // Waypoint 0 : (2, 1) face à droite
            {2.0, -1.0, 1.57},   // Waypoint 1 : (2, -1) face en haut
            {-1.0, -1.0, 3.14},  // Waypoint 2 : (-1, -1) face à gauche
            {-1.0,  1.0, -1.57}, // Waypoint 3 : (-1, 1) face en bas
            {0.0,  0.0, 0.0}     // Waypoint 4 : retour à l'origine
        };
        // Ces waypoints forment un circuit dans notre monde Gazebo

        // --------------------------------------------------------
        // CRÉER LE CLIENT D'ACTION
        // --------------------------------------------------------
        client_ = rclcpp_action::create_client<NavigateToPose>(
            this, "navigate_to_pose"
        );

        RCLCPP_INFO(this->get_logger(),
            "WaypointSender ready — %zu waypoints to visit",
            waypoints_.size());

        // Attendre Nav2 avant d'envoyer le premier waypoint
        timer_ = this->create_wall_timer(
            std::chrono::seconds(1),
            std::bind(&WaypointSender::checkAndStart, this)
        );
    }

private:

    // Structure pour stocker un waypoint
    struct Waypoint {
        double x, y, yaw;
    };

    // --------------------------------------------------------
    // ATTENDRE NAV2 ET DÉMARRER
    // --------------------------------------------------------
    void checkAndStart()
    {
        if (!client_->wait_for_action_server(std::chrono::seconds(0)))
        {
            RCLCPP_WARN(this->get_logger(), "Waiting for Nav2...");
            return;
        }

        timer_->cancel();
        RCLCPP_INFO(this->get_logger(),
            "Nav2 ready ! Starting waypoint mission...");

        // Envoyer le premier waypoint
        sendNextWaypoint();
    }

    // --------------------------------------------------------
    // ENVOYER LE PROCHAIN WAYPOINT
    // --------------------------------------------------------
    void sendNextWaypoint()
    {
        // Vérifier si tous les waypoints ont été visités
        if (current_waypoint_ >= waypoints_.size())
        {
            RCLCPP_INFO(this->get_logger(),
                "🏁 Mission complete ! All %zu waypoints visited !",
                waypoints_.size());
            return;
        }

        // Récupérer le waypoint actuel
        auto & wp = waypoints_[current_waypoint_];

        RCLCPP_INFO(this->get_logger(),
            "📍 Navigating to waypoint %zu/%zu : (%.2f, %.2f)",
            current_waypoint_ + 1, waypoints_.size(), wp.x, wp.y);

        // --------------------------------------------------------
        // CONSTRUIRE LE GOAL
        // --------------------------------------------------------
        auto goal_msg = NavigateToPose::Goal();
        goal_msg.pose.header.frame_id = "map";
        goal_msg.pose.header.stamp    = this->get_clock()->now();

        goal_msg.pose.pose.position.x = wp.x;
        goal_msg.pose.pose.position.y = wp.y;
        goal_msg.pose.pose.position.z = 0.0;

        // Convertir yaw en quaternion
        goal_msg.pose.pose.orientation.z = std::sin(wp.yaw / 2.0);
        goal_msg.pose.pose.orientation.w = std::cos(wp.yaw / 2.0);

        // --------------------------------------------------------
        // CALLBACKS
        // --------------------------------------------------------
        auto options = rclcpp_action::Client<NavigateToPose>::SendGoalOptions();

        options.goal_response_callback =
            std::bind(&WaypointSender::goalResponseCallback, this,
                std::placeholders::_1);

        options.feedback_callback =
            std::bind(&WaypointSender::feedbackCallback, this,
                std::placeholders::_1, std::placeholders::_2);

        options.result_callback =
            std::bind(&WaypointSender::resultCallback, this,
                std::placeholders::_1);

        client_->async_send_goal(goal_msg, options);
    }

    // --------------------------------------------------------
    // CALLBACK 1 : réponse du serveur
    // --------------------------------------------------------
    void goalResponseCallback(const GoalHandle::SharedPtr & goal_handle)
    {
        if (!goal_handle)
        {
            RCLCPP_ERROR(this->get_logger(),
                "Waypoint %zu rejected ! Skipping...",
                current_waypoint_ + 1);
            // Passer au waypoint suivant même si rejeté
            current_waypoint_++;
            sendNextWaypoint();
        }
        else
        {
            RCLCPP_INFO(this->get_logger(),
                "Waypoint %zu accepted !",
                current_waypoint_ + 1);
        }
    }

    // --------------------------------------------------------
    // CALLBACK 2 : feedback
    // --------------------------------------------------------
    void feedbackCallback(
        GoalHandle::SharedPtr,
        const std::shared_ptr<const NavigateToPose::Feedback> feedback)
    {
        RCLCPP_INFO_THROTTLE(
            this->get_logger(),
            *this->get_clock(),
            2000,
            // Afficher le feedback toutes les 2 secondes
            // pour ne pas surcharger le terminal
            "Waypoint %zu — Distance remaining: %.2fm",
            current_waypoint_ + 1,
            feedback->distance_remaining
        );
    }

    // --------------------------------------------------------
    // CALLBACK 3 : résultat final du waypoint actuel
    // --------------------------------------------------------
    void resultCallback(const GoalHandle::WrappedResult & result)
    {
        switch (result.code)
        {
            case rclcpp_action::ResultCode::SUCCEEDED:
                RCLCPP_INFO(this->get_logger(),
                    "✅ Waypoint %zu reached !",
                    current_waypoint_ + 1);
                // Passer au waypoint suivant
                current_waypoint_++;
                sendNextWaypoint();
                break;

            case rclcpp_action::ResultCode::ABORTED:
                RCLCPP_ERROR(this->get_logger(),
                    "❌ Waypoint %zu aborted ! Trying next...",
                    current_waypoint_ + 1);
                // Passer au waypoint suivant même en cas d'échec
                current_waypoint_++;
                sendNextWaypoint();
                break;

            case rclcpp_action::ResultCode::CANCELED:
                RCLCPP_WARN(this->get_logger(),
                    "⚠️  Waypoint %zu canceled !",
                    current_waypoint_ + 1);
                break;

            default:
                RCLCPP_ERROR(this->get_logger(), "Unknown result !");
                break;
        }
    }

    // --------------------------------------------------------
    // ATTRIBUTS PRIVÉS
    // --------------------------------------------------------
    rclcpp_action::Client<NavigateToPose>::SharedPtr client_;
    rclcpp::TimerBase::SharedPtr timer_;

    std::vector<Waypoint> waypoints_;
    // Liste de tous les waypoints à visiter

    size_t current_waypoint_;
    // Index du waypoint actuel
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<WaypointSender>());
    rclcpp::shutdown();
    return 0;
}
