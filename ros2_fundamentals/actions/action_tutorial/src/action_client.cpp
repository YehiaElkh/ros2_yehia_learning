#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "custom_msg_and_srv/action/concatenate.hpp"

class ActionClient : public rclcpp::Node
{
public:
    using Concatenate = custom_msg_and_srv::action::Concatenate;
    using GoalHandle  = rclcpp_action::ClientGoalHandle<Concatenate>;

    ActionClient() : Node("action_client")
    {
        // ----------------------------------------------------
        // CRÉER LE CLIENT D'ACTION
        // ----------------------------------------------------
        client_ = rclcpp_action::create_client<Concatenate>(
            this,
            "concatenate"   // même nom que le server
        );

        // Attendre que le server soit disponible
        while (!client_->wait_for_action_server(std::chrono::seconds(1)))
        {
            RCLCPP_WARN(this->get_logger(), "Waiting for action server...");
        }

        RCLCPP_INFO(this->get_logger(), "Action server found ! Sending goal...");

        // ----------------------------------------------------
        // CRÉER ET ENVOYER LE GOAL
        // ----------------------------------------------------
        auto goal = Concatenate::Goal();
        goal.first_string  = "Hello";
        goal.second_string = "ROS2 Actions !";

        // Options : définir les callbacks
        auto options = rclcpp_action::Client<Concatenate>::SendGoalOptions();

        // Callback quand le goal est accepté/rejeté
        options.goal_response_callback =
            std::bind(&ActionClient::goalResponseCallback, this,
                std::placeholders::_1);

        // Callback quand on reçoit un feedback
        options.feedback_callback =
            std::bind(&ActionClient::feedbackCallback, this,
                std::placeholders::_1, std::placeholders::_2);

        // Callback quand le goal est terminé
        options.result_callback =
            std::bind(&ActionClient::resultCallback, this,
                std::placeholders::_1);

        client_->async_send_goal(goal, options);
    }

private:
    rclcpp_action::Client<Concatenate>::SharedPtr client_;

    // --------------------------------------------------------
    // CALLBACK 1 : réponse du server (accepté/rejeté)
    // --------------------------------------------------------
    void goalResponseCallback(const GoalHandle::SharedPtr & goal_handle)
    {
        if (!goal_handle)
            RCLCPP_ERROR(this->get_logger(), "Goal rejected !");
        else
            RCLCPP_INFO(this->get_logger(), "Goal accepted !");
    }

    // --------------------------------------------------------
    // CALLBACK 2 : feedback reçu pendant l'exécution
    // --------------------------------------------------------
    void feedbackCallback(
        GoalHandle::SharedPtr,
        const std::shared_ptr<const Concatenate::Feedback> feedback)
    {
        RCLCPP_INFO(this->get_logger(), "Feedback: %s",
            feedback->current_status.c_str());
    }

    // --------------------------------------------------------
    // CALLBACK 3 : résultat final
    // --------------------------------------------------------
    void resultCallback(const GoalHandle::WrappedResult & result)
    {
        switch (result.code)
        {
            case rclcpp_action::ResultCode::SUCCEEDED:
                RCLCPP_INFO(this->get_logger(), "Result: '%s'",
                    result.result->concatenated_string.c_str());
                break;
            case rclcpp_action::ResultCode::CANCELED:
                RCLCPP_WARN(this->get_logger(), "Goal was canceled !");
                break;
            case rclcpp_action::ResultCode::ABORTED:
                RCLCPP_ERROR(this->get_logger(), "Goal was aborted !");
                break;
            default:
                break;
        }
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ActionClient>());
    rclcpp::shutdown();
    return 0;
}