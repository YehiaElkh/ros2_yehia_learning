import time
import rclpy
from rclpy.node import Node
from rclpy.action import ActionServer
from custom_msg_and_srv.action import Concatenate

class ConcatenateActionServer(Node):
    def __init__(self):
        super().__init__("action_server")

        # ----------------------------------------------------
        # CRÉER LE SERVER D'ACTION
        # ----------------------------------------------------
        self.server_ = ActionServer(
            self,
            Concatenate,          # type de l'action
            "concatenate",        # nom de l'action
            self.execute_callback # callback d'exécution
        )

        self.get_logger().info("Action Server started !")

    # --------------------------------------------------------
    # CALLBACK D'EXÉCUTION
    # appelé automatiquement quand un goal est reçu
    # --------------------------------------------------------
    def execute_callback(self, goal_handle):
        self.get_logger().info(
            f"Executing goal: '{goal_handle.request.first_string}'"
            f" + '{goal_handle.request.second_string}'"
        )

        feedback = Concatenate.Feedback()

        # Simuler une tâche longue en 3 étapes
        for step in range(1, 4):
            # Vérifier si annulé
            if goal_handle.is_cancel_requested:
                goal_handle.canceled()
                self.get_logger().warn("Goal canceled !")
                return Concatenate.Result()

            # Envoyer un feedback
            feedback.current_status = f"Step {step}/3 processing..."
            goal_handle.publish_feedback(feedback)
            self.get_logger().info(f"Feedback: {feedback.current_status}")

            time.sleep(1)   # attendre 1 seconde

        # Tâche terminée → résultat final
        result = Concatenate.Result()
        result.concatenated_string = (
            goal_handle.request.first_string + " " +
            goal_handle.request.second_string
        )

        goal_handle.succeed()
        self.get_logger().info(f"Goal succeeded ! Result: '{result.concatenated_string}'")
        return result

def main(args=None):
    rclpy.init(args=args)
    node = ConcatenateActionServer()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == "__main__":
    main()