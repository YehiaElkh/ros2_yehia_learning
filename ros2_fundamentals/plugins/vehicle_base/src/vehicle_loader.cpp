#include "rclcpp/rclcpp.hpp"
#include "pluginlib/class_loader.hpp"
//       | bibliothèque pour charger les plugins dynamiquement
#include "vehicle_base/vehicle_base.hpp"

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("vehicle_loader");

    // --------------------------------------------------------
    // CRÉER LE CLASS LOADER
    // --------------------------------------------------------
    // ClassLoader<BaseClass>(package, base_class_name)
    pluginlib::ClassLoader<vehicle_base::VehicleBase> loader(
        "vehicle_base",                    // package qui contient la classe de base
        "vehicle_base::VehicleBase"        // nom complet de la classe de base
    );

    try
    {
        // ----------------------------------------------------
        // CHARGER UN PLUGIN DYNAMIQUEMENT
        // ----------------------------------------------------
        // On peut changer "vehicle_plugins::Motorbike" par
        // n'importe quel plugin sans recompiler !
        std::string plugin_name = "vehicle_plugins::Motorbike";

        RCLCPP_INFO(node->get_logger(), "Loading plugin: %s", plugin_name.c_str());

        auto vehicle = loader.createSharedInstance(plugin_name);
        //             ↑ charge et instancie le plugin

        // Utiliser le plugin via l'interface de base
        RCLCPP_INFO(node->get_logger(), "Info : %s", vehicle->getInfo().c_str());
        vehicle->move();
        vehicle->stop();

        // ----------------------------------------------------
        // CHARGER UN AUTRE PLUGIN sans recompiler
        // ----------------------------------------------------
        plugin_name = "vehicle_plugins::Bicycle";
        RCLCPP_INFO(node->get_logger(), "Loading plugin: %s", plugin_name.c_str());

        auto vehicle2 = loader.createSharedInstance(plugin_name);
        RCLCPP_INFO(node->get_logger(), "Info : %s", vehicle2->getInfo().c_str());
        vehicle2->move();
        vehicle2->stop();

        // ----------------------------------------------------
        // CHARGER UN TROISIÈME PLUGIN
        // ----------------------------------------------------
        plugin_name = "vehicle_plugins::Truck";
        RCLCPP_INFO(node->get_logger(), "Loading plugin: %s", plugin_name.c_str());

        auto vehicle3 = loader.createSharedInstance(plugin_name);
        RCLCPP_INFO(node->get_logger(), "Info : %s", vehicle3->getInfo().c_str());
        vehicle3->move();
        vehicle3->stop();
    }
    catch (const pluginlib::PluginlibException & e)
    {
        RCLCPP_ERROR(node->get_logger(), "Plugin error: %s", e.what());
    }

    rclcpp::shutdown();
    return 0;
}