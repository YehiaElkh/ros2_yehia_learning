import rclpy
from rclpy.node import Node

def main(args=None):
    rclpy.init(args=args)

    node = Node("my_first_node")
    node.get_logger().info("Hello from my_first_node !")

    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == "__main__":
    main()