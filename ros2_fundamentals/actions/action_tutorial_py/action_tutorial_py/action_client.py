import rclpy
from rclpy.node import Node
from rclpy.action import ActionClient
from custom_msg_and_srv.action import Concatenate

class ConcatenateActionClient(Node):
    def __init__(self):
        super().__init__("action_client")

        # ----------------------------------------------------
        # CRÉER LE CLIENT D'ACTION
        # ----------------------------------------------------
        self.client_ = ActionClient(
            self,
            Concatenate,     # type de l'action
            "concatenate"    # nom de l'action
        )

        # Attendre que le server soit disponible
        while not self.client_.wait_for_server(timeout_sec=1.0):
            self.get_logger().warn("Waiting for action server...")

        self.get_logger().info("Action server found ! Sending goal...")

        # ----------------------------------------------------
        # CRÉER ET ENVOYER LE GOAL
        # ----------------------------------------------------
        goal = Concatenate.Goal()
        goal.first_string  = "Hello"
        goal.second_string = "ROS2 Actions !"

        # Envoyer le goal avec les callbacks
        self.client_.send_goal_async(
            goal,
            feedback_callback=self.feedback_callback
        ).add_done_callback(self.goal_response_callback)

    # --------------------------------------------------------
    # CALLBACK 1 : réponse du server
    # --------------------------------------------------------
    def goal_response_callback(self, future):
        goal_handle = future.result()

        if not goal_handle.accepted:
            self.get_logger().error("Goal rejected !")
            return

        self.get_logger().info("Goal accepted !")

        # Attendre le résultat final
        goal_handle.get_result_async().add_done_callback(
            self.result_callback
        )

    # --------------------------------------------------------
    # CALLBACK 2 : feedback pendant l'exécution
    # --------------------------------------------------------
    def feedback_callback(self, feedback_msg):
        self.get_logger().info(
            f"Feedback: {feedback_msg.feedback.current_status}"
        )

    # --------------------------------------------------------
    # CALLBACK 3 : résultat final
    # --------------------------------------------------------
    def result_callback(self, future):
        result = future.result().result
        self.get_logger().info(f"Result: '{result.concatenated_string}'")

def main(args=None):
    rclpy.init(args=args)
    node = ConcatenateActionClient()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == "__main__":
    main()