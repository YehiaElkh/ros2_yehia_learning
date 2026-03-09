import rclpy
from rclpy.node import Node
from publisher_library_py.timed_publisher import TimedPublisher
#    | importer la librairie

class UseLibrary(Node):
    def __init__(self):
        super().__init__("use_library")

        self.get_logger().info("UseLibrary node started !")

        # --------------------------------------------------------
        # UTILISER LA LIBRAIRIE
        # --------------------------------------------------------
        # Publisher rapide
        self.publisher1_ = TimedPublisher(
            self,          #  le nœud actuel
            "topic_fast",  #  topic
            0.5            #  période 500ms
        )
        self.publisher1_.set_message("Fast message")

        # Publisher lent
        self.publisher2_ = TimedPublisher(
            self,
            "topic_slow",
            2.0   #  période 2s
        )
        self.publisher2_.set_message("Slow message")

def main(args=None):
    rclpy.init(args=args)
    node = UseLibrary()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == "__main__":
    main()