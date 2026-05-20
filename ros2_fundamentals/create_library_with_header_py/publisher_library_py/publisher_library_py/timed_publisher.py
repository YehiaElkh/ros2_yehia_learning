from rclpy.node import Node
from std_msgs.msg import String

# ============================================================
# CLASSE DE LA LIBRAIRIE Python
# ============================================================

class TimedPublisher:
    def __init__(self, node: Node, topic: str, period_seconds: float):
        """
        node           : nœud ROS2 existant
        topic          : nom du topic
        period_seconds : période du timer en secondes
        """
        self.node_    = node
        self.message_ = "Default message"
        self.count_   = 0

        # Créer le publisher sur le nœud fourni
        self.publisher_ = node.create_publisher(String, topic, 10)

        # Créer le timer
        self.timer_ = node.create_timer(period_seconds, self.timer_callback)

        node.get_logger().info(
            f"TimedPublisher created on topic '{topic}' "
            f"with period {period_seconds}s"
        )

    def start(self):
        """Démarrer la publication"""
        if self.timer_.is_canceled():
            self.timer_.reset()
            self.node_.get_logger().info("TimedPublisher started !")

    def stop(self):
        """Arrêter la publication"""
        self.timer_.cancel()
        self.node_.get_logger().info("TimedPublisher stopped !")

    def set_message(self, message: str):
        """Changer le message publié"""
        self.message_ = message
        self.node_.get_logger().info(f"Message changed to: '{message}'")

    def timer_callback(self):
        msg = String()
        msg.data = f"{self.message_} [{self.count_}]"
        self.count_ += 1
        self.publisher_.publish(msg)
        self.node_.get_logger().info(f"Publishing: '{msg.data}'")