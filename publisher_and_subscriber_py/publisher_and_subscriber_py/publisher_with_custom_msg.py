import rclpy
from rclpy.node import Node
from custom_msg_and_srv.msg import EmployeeSalary

class PublisherCustomMsg(Node):
    def __init__(self):
        super().__init__("publisher_custom_msg")
        self.count_ = 0
        self.publisher_ = self.create_publisher(EmployeeSalary, "employee", 10)
        self.timer_ = self.create_timer(0.5, self.timer_callback)
        self.get_logger().info("Publisher Custom Msg started !")

    def timer_callback(self):
        msg = EmployeeSalary()
        msg.first_name = "John"
        msg.last_name  = "Doe"
        msg.salary     = 3000.0 + self.count_ * 100.0
        self.count_ += 1
        self.get_logger().info(f"Publishing: {msg.first_name} {msg.last_name} - {msg.salary:.2f}")
        self.publisher_.publish(msg)

def main(args=None):
    rclpy.init(args=args)
    node = PublisherCustomMsg()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == "__main__":
    main()