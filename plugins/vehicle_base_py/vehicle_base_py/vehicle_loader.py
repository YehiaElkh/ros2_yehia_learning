import rclpy
from rclpy.node import Node
import importlib.metadata
# importlib.metadata = bibliothèque Python pour lire les entry points
# c'est le mécanisme qui remplace pluginlib en Python

class VehicleLoader(Node):
    def __init__(self):
        super().__init__("vehicle_loader")

        # --------------------------------------------------------
        # CHARGER LES PLUGINS via entry points
        # --------------------------------------------------------
        # On cherche tous les plugins enregistrés sous le groupe
        # "vehicle_base_py.plugins" dans les entry_points de setup.py
        self.get_logger().info("Loading all vehicle plugins...")

        # Récupérer tous les entry points du groupe
        plugins = importlib.metadata.entry_points(
            group="vehicle_base_py.plugins"
            #      ↑ nom du groupe défini dans setup.py
        )

        # Charger et utiliser chaque plugin
        for plugin in plugins:
            try:
                # Charger la classe du plugin
                plugin_class = plugin.load()
                #               ↑ importe et retourne la classe

                # Instancier le plugin
                vehicle = plugin_class()

                # Utiliser le plugin via l'interface de base
                self.get_logger().info(f"Loading plugin: {plugin.name}")
                self.get_logger().info(f"Info : {vehicle.get_info()}")
                vehicle.move()
                vehicle.stop()
                self.get_logger().info("---")

            except Exception as e:
                self.get_logger().error(f"Error loading plugin {plugin.name}: {e}")

def main(args=None):
    rclpy.init(args=args)
    node = VehicleLoader()
    rclpy.shutdown()

if __name__ == "__main__":
    main()