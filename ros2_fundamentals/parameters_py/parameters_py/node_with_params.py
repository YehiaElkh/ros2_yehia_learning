import rclpy
from rclpy.node import Node

class NodeWithParams(Node):
    def __init__(self):
        super().__init__("node_with_params")


        # DÉCLARER LES PARAMÈTRES
        self.declare_parameter("timer_period", 1.0)
        #                       |               |
        #                      nom         valeur par défaut

        self.declare_parameter("message", "Hello ROS2 !")
        self.declare_parameter("max_count", 10)


        # LIRE LES PARAMÈTRES
        timer_period = self.get_parameter("timer_period").get_parameter_value().double_value
        #                                                  |
        #                                  get_parameter_value() : récupérer la valeur
        #                                  .double_value  : en double
        #                                  .string_value  : en string
        #                                  .integer_value : en int
        #                                  .bool_value    : en bool

        self.message_   = self.get_parameter("message").get_parameter_value().string_value
        self.max_count_ = self.get_parameter("max_count").get_parameter_value().integer_value
        self.count_ = 0

        self.get_logger().info("Parameters loaded :")
        self.get_logger().info(f"  timer_period : {timer_period}")
        self.get_logger().info(f"  message      : {self.message_}")
        self.get_logger().info(f"  max_count    : {self.max_count_}")


        # CRÉER LE TIMER avec la période du paramètre
        self.timer_ = self.create_timer(timer_period, self.timer_callback)


    # CALLBACK DU TIMER
    def timer_callback(self):
        if self.count_ >= self.max_count_:
            self.get_logger().warn("Max count reached ! Stopping...")
            self.timer_.cancel()   # arrêter le timer
            return

        self.count_ += 1
        self.get_logger().info(f"[{self.count_}/{self.max_count_}] {self.message_}")

def main(args=None):
    rclpy.init(args=args)
    node = NodeWithParams()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == "__main__":
    main()