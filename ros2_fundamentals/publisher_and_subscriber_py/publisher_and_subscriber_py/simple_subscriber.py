import rclpy
from rclpy.node import Node
from std_msgs.msg import String

class SimpleSubscriber(Node):
    def __init__(self):
        super().__init__("simple_subscriber")

        # ----------------------------------------------------
        # CRÉER LE SUBSCRIBER
        # ----------------------------------------------------
        self.subscriber_ = self.create_subscription(
            String,                  # type du message (doit matcher le publisher)
            "chatter",               # nom du topic (doit matcher le publisher)
            self.message_callback,   # fonction appelée à chaque message reçu
            10                       # QoS
        )
        self.get_logger().info("Subscriber started !")

    # --------------------------------------------------------
    # CALLBACK DU SUBSCRIBER
    # --------------------------------------------------------
    def message_callback(self, msg):
        # msg est automatiquement passé par ROS2 quand un message arrive
        self.get_logger().info(f"Received: '{msg.data}'")

def main(args=None):
    rclpy.init(args=args)
    node = SimpleSubscriber()
    rclpy.spin(node)
    rclpy.shutdown()
