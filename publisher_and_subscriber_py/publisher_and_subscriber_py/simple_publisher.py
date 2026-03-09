import rclpy                        # bibliothèque principale ROS2 Python
from rclpy.node import Node         # classe de base pour tous les nœuds
from std_msgs.msg import String     # type de message

class SimplePublisher(Node):        # notre classe hérite de Node
    def __init__(self):
        super().__init__("simple_publisher")
        # super().__init__ : appeler le constructeur de Node avec le nom du nœud

        self.count_ = 0             # attribut de l'instance, initialisé à 0

        
        # CRÉER LE PUBLISHER
        self.publisher_ = self.create_publisher(
            String,      # type du message
            "chatter",   # nom du topic
            10           # QoS
        )

       
        # CRÉER LE TIMER
        self.timer_ = self.create_timer(
            0.5,                    # période en secondes (0.5s = 500ms)
            self.timer_callback     # fonction à appeler quand le timer sonne
        )

        self.get_logger().info("Publisher started !")

    # CALLBACK DU TIMER
    def timer_callback(self):
        msg = String()                              # créer un message vide
        msg.data = f"Hello ROS2 n:{self.count_}"   # remplir le message
        self.count_ += 1                            # incrémenter le compteur
        self.get_logger().info(f"Publishing: '{msg.data}'")
        self.publisher_.publish(msg)                # publier !

def main(args=None):
    rclpy.init(args=args)           # initialiser ROS2
    node = SimplePublisher()        # créer le nœud
    rclpy.spin(node)                # boucle infinie
    rclpy.shutdown()                # nettoyage