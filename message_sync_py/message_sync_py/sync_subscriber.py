import rclpy
from rclpy.node import Node
from std_msgs.msg import String, Int32

# ============================================================
# message_filters en Python
# ============================================================
import message_filters

class SyncSubscriber(Node):
    def __init__(self):
        super().__init__("sync_subscriber")

        # --------------------------------------------------------
        # CRÉER LES SUBSCRIBERS avec message_filters
        # --------------------------------------------------------
        self.sub1_ = message_filters.Subscriber(self, String, "camera_data")
        self.sub2_ = message_filters.Subscriber(self, Int32,  "lidar_data")

        # --------------------------------------------------------
        # CRÉER LE SYNCHRONIZER
        # --------------------------------------------------------
        self.sync_ = message_filters.ApproximateTimeSynchronizer(
            [self.sub1_, self.sub2_],   # liste des subscribers
            queue_size=10,              # messages en attente
            slop=0.1                    # tolérance en secondes
            # slop = écart maximum accepté entre les timestamps
        )

        # Enregistrer le callback
        self.sync_.registerCallback(self.sync_callback)

        self.get_logger().info("Sync Subscriber started !")

    # --------------------------------------------------------
    # CALLBACK DE SYNCHRONISATION
    # --------------------------------------------------------
    def sync_callback(self, msg1, msg2):
        self.get_logger().info(
            f"SYNCED → camera: '{msg1.data}' | lidar: {msg2.data}"
        )

def main(args=None):
    rclpy.init(args=args)
    node = SyncSubscriber()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == "__main__":
    main()