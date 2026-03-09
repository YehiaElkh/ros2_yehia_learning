import math
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import TransformStamped
from tf2_ros import TransformBroadcaster

class DynamicTF2Publisher(Node):
    def __init__(self):
        super().__init__("dynamic_tf2_publisher")

        # ----------------------------------------------------
        # DÉCLARER LES PARAMÈTRES
        # ----------------------------------------------------
        self.declare_parameter("parent_frame", "map")
        self.declare_parameter("child_frame",  "base_link")
        self.declare_parameter("x",   0.0)
        self.declare_parameter("y",   0.0)
        self.declare_parameter("z",   0.0)
        self.declare_parameter("yaw", 0.0)

        # Lire les paramètres
        self.parent_frame_ = self.get_parameter("parent_frame").get_parameter_value().string_value
        self.child_frame_  = self.get_parameter("child_frame").get_parameter_value().string_value
        self.x_   = self.get_parameter("x").get_parameter_value().double_value
        self.y_   = self.get_parameter("y").get_parameter_value().double_value
        self.z_   = self.get_parameter("z").get_parameter_value().double_value
        self.yaw_ = self.get_parameter("yaw").get_parameter_value().double_value

        # ----------------------------------------------------
        # CRÉER LE BROADCASTER TF2
        # ----------------------------------------------------
        self.tf_broadcaster_ = TransformBroadcaster(self)

        # Angle courant pour l'animation
        self.angle_ = 0.0

        # Timer : publier toutes les 100ms
        self.timer_ = self.create_timer(0.1, self.timer_callback)

        self.get_logger().info(
            f"TF2 Publisher started : {self.parent_frame_} → {self.child_frame_}"
        )

    def timer_callback(self):
        # ----------------------------------------------------
        # CRÉER LE MESSAGE DE TRANSFORMATION
        # ----------------------------------------------------
        transform = TransformStamped()

        # En-tête
        transform.header.stamp    = self.get_clock().now().to_msg()
        transform.header.frame_id = self.parent_frame_
        transform.child_frame_id  = self.child_frame_

        # ----------------------------------------------------
        # TRANSLATION
        # ----------------------------------------------------
        transform.transform.translation.x = self.x_
        transform.transform.translation.y = self.y_
        transform.transform.translation.z = self.z_

        # ----------------------------------------------------
        # ROTATION (quaternion depuis yaw)
        # ----------------------------------------------------
        transform.transform.rotation.x = 0.0
        transform.transform.rotation.y = 0.0
        transform.transform.rotation.z = math.sin(self.angle_ / 2.0)
        transform.transform.rotation.w = math.cos(self.angle_ / 2.0)

        # Animation : faire tourner lentement
        self.angle_ += 0.01
        if self.angle_ > 2 * math.pi:
            self.angle_ = 0.0

        # Publier la transformation
        self.tf_broadcaster_.sendTransform(transform)

def main(args=None):
    rclpy.init(args=args)
    node = DynamicTF2Publisher()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == "__main__":
    main()