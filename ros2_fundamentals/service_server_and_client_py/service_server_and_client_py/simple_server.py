import rclpy
from rclpy.node import Node
from custom_msg_and_srv.srv import AddTwoInts

class SimpleServer(Node):
    def __init__(self):
        super().__init__("simple_server")


        # CRÉER LE SERVICE
        self.server_ = self.create_service(
            AddTwoInts,              # type du service
            "add_two_ints",          # nom du service
            self.service_callback    # callback appelé à chaque requête
        )

        self.get_logger().info("Server started, waiting for requests...")

    # CALLBACK DU SERVICE
    def service_callback(self, request, response):
        # request : ce que le client envoie (a et b)
        # response : ce qu'on retourne (sum)

        response.sum = request.a + request.b

        self.get_logger().info(
            f"Request received: a={request.a}, b={request.b} → sum={response.sum}"
        )

        return response   # IMPORTANT : toujours retourner la response !

def main(args=None):
    rclpy.init(args=args)
    node = SimpleServer()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == "__main__":
    main()