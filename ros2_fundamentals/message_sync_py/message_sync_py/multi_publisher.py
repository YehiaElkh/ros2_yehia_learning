import rclpy
from rclpy.node import Node
from std_msgs.msg import String, Int32

class MultiPublisher(Node):
    def __init__(self):
        super().__init__("multi_publisher")
        self.count_ = 0

        # Publisher 1 : simule une caméra
        self.publisher1_ = self.create_publisher(String, "camera_data", 10)

        # Publisher 2 : simule un LiDAR
        self.publisher2_ = self.create_publisher(Int32, "lidar_data", 10)

        # Timer : publier toutes les 500ms
        self.timer_ = self.create_timer(0.5, self.timer_callback)

        self.get_logger().info("Multi Publisher started !")

    def timer_callback(self):
        # Message caméra
        msg1 = String()
        msg1.data = f"Camera frame n°{self.count_}"

        # Message LiDAR
        msg2 = Int32()
        msg2.data = self.count_ * 10

        self.count_ += 1

        # Publier les deux messages
        self.publisher1_.publish(msg1)
        self.publisher2_.publish(msg2)

        self.get_logger().info(
            f"Published: '{msg1.data}' | lidar: {msg2.data}"
        )

def main(args=None):
    rclpy.init(args=args)
    node = MultiPublisher()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == "__main__":
    main()