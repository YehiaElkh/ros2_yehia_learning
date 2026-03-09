#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
//       ↑ bibliothèque spécifique aux actions

#include "custom_msg_and_srv/action/concatenate.hpp"

class ActionServer : public rclcpp::Node
{
public:
    // Alias pour simplifier l'écriture
    using Concatenate = custom_msg_and_srv::action::Concatenate;
    using GoalHandle  = rclcpp_action::ServerGoalHandle<Concatenate>;

    ActionServer() : Node("action_server")
    {
        // ----------------------------------------------------
        // CRÉER LE SERVER D'ACTION
        // ----------------------------------------------------
        server_ = rclcpp_action::create_server<Concatenate>(
            this,
            "concatenate",   // nom de l'action

            // Callback 1 : quand un goal arrive
            std::bind(&ActionServer::handleGoal, this,
                std::placeholders::_1, std::placeholders::_2),

            // Callback 2 : quand le client annule le goal
            std::bind(&ActionServer::handleCancel, this,
                std::placeholders::_1),

            // Callback 3 : quand le goal est accepté et prêt à s'exécuter
            std::bind(&ActionServer::handleAccepted, this,
                std::placeholders::_1)
        );

        RCLCPP_INFO(this->get_logger(), "Action Server started !");
    }

private:
    rclcpp_action::Server<Concatenate>::SharedPtr server_;

    // --------------------------------------------------------
    // CALLBACK 1 : décider d'accepter ou rejeter le goal
    // --------------------------------------------------------
    rclcpp_action::GoalResponse handleGoal(
        const rclcpp_action::GoalUUID & uuid,
        std::shared_ptr<const Concatenate::Goal> goal)
    {
        (void)uuid;   // non utilisé ici
        RCLCPP_INFO(this->get_logger(),
            "Goal received: '%s' + '%s'",
            goal->first_string.c_str(),
            goal->second_string.c_str());

        // Accepter le goal
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
        // ou rejeter : return rclcpp_action::GoalResponse::REJECT;
    }

    // --------------------------------------------------------
    // CALLBACK 2 : gérer l'annulation
    // --------------------------------------------------------
    rclcpp_action::CancelResponse handleCancel(
        const std::shared_ptr<GoalHandle> goal_handle)
    {
        (void)goal_handle;
        RCLCPP_WARN(this->get_logger(), "Goal cancelled !");
        return rclcpp_action::CancelResponse::ACCEPT;
    }

    // --------------------------------------------------------
    // CALLBACK 3 : exécuter le goal dans un thread séparé
    // --------------------------------------------------------
    void handleAccepted(const std::shared_ptr<GoalHandle> goal_handle)
    {
        // Lancer l'exécution dans un thread séparé
        // pour ne pas bloquer le nœud principal
        std::thread{std::bind(&ActionServer::execute, this,
            std::placeholders::_1), goal_handle}.detach();
    }

    // --------------------------------------------------------
    // EXÉCUTION DU GOAL
    // --------------------------------------------------------
    void execute(const std::shared_ptr<GoalHandle> goal_handle)
    {
        RCLCPP_INFO(this->get_logger(), "Executing goal...");

        const auto goal = goal_handle->get_goal();
        auto feedback   = std::make_shared<Concatenate::Feedback>();
        auto result     = std::make_shared<Concatenate::Result>();

        rclcpp::Rate rate(1);   // 1 Hz = 1 fois par seconde

        // Simuler une tâche longue en 3 étapes
        for (int step = 1; step <= 3; step++)
        {
            // Vérifier si le goal a été annulé
            if (goal_handle->is_canceling())
            {
                result->concatenated_string = "";
                goal_handle->canceled(result);
                RCLCPP_WARN(this->get_logger(), "Goal was canceled !");
                return;
            }

            // Envoyer un feedback
            feedback->current_status = "Step " + std::to_string(step) + "/3 processing...";
            goal_handle->publish_feedback(feedback);
            RCLCPP_INFO(this->get_logger(), "Feedback: %s",
                feedback->current_status.c_str());

            rate.sleep();   // attendre 1 seconde
        }

        // Tâche terminée → envoyer le résultat
        result->concatenated_string = goal->first_string + " " + goal->second_string;
        goal_handle->succeed(result);
        RCLCPP_INFO(this->get_logger(), "Goal succeeded ! Result: '%s'",
            result->concatenated_string.c_str());
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ActionServer>());
    rclcpp::shutdown();
    return 0;
}