import rclpy
from rclpy.node import Node

class MyNode(Node):
    def __init__(self):
        super().__init__("node_with_timer")
        self.count_ = 0
        self.timer_ = self.create_timer(0.5, self.timer_callback)
        self.get_logger().info("Node started !")

    def timer_callback(self):
        self.count_ += 1
        self.get_logger().info(f"Hello n°{self.count_}")

def main(args=None):
    rclpy.init(args=args)
    node = MyNode()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == "__main__":
    main()