import rclpy
from rclpy.node import Node
from custom_msg_and_srv.srv import AddTwoInts

class SimpleClient(Node):
    def __init__(self):
        super().__init__("simple_client")

        # CRÉER LE CLIENT
        self.client_ = self.create_client(
            AddTwoInts,      # type du service
            "add_two_ints"   # nom du service
        )

        # ATTENDRE QUE LE SERVEUR SOIT DISPONIBLE
        while not self.client_.wait_for_service(timeout_sec=1.0):
            self.get_logger().warn("Waiting for server...")

        self.get_logger().info("Server found ! Sending request...")


        # CRÉER ET ENVOYER LA REQUÊTE
        request = AddTwoInts.Request()
        request.a = 10
        request.b = 25

        # Envoyer la requête de façon ASYNCHRONE
        self.future_ = self.client_.call_async(request)

        # Enregistrer un callback pour quand la réponse arrive
        self.future_.add_done_callback(self.response_callback)

    # CALLBACK DE LA RÉPONSE
    def response_callback(self, future):
        response = future.result()
        # future.result() : récupérer la réponse du serveur

        self.get_logger().info(f"Response received: sum = {response.sum}")

def main(args=None):
    rclpy.init(args=args)
    node = SimpleClient()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == "__main__":
    main()