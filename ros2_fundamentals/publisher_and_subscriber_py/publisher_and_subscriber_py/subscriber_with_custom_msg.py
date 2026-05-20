import rclpy
from rclpy.node import Node
from custom_msg_and_srv.msg import EmployeeSalary

class SubscriberCustomMsg(Node):
    def __init__(self):
        super().__init__("subscriber_custom_msg")
        self.subscriber_ = self.create_subscription(
            EmployeeSalary,
            "employee",
            self.message_callback,
            10
        )
        self.get_logger().info("Subscriber Custom Msg started !")

    def message_callback(self, msg):
        self.get_logger().info(
            f"Received: {msg.first_name} {msg.last_name} - {msg.salary:.2f}"
        )

def main(args=None):
    rclpy.init(args=args)
    node = SubscriberCustomMsg()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == "__main__":
    main()