#include "vehicle_plugins/truck.hpp"
#include "pluginlib/class_list_macros.hpp"

namespace vehicle_plugins
{

void Truck::move()
{
    RCLCPP_INFO(rclcpp::get_logger("Truck"), "Truck is moving heavily ! 🚛");
}

void Truck::stop()
{
    RCLCPP_INFO(rclcpp::get_logger("Truck"), "Truck stopped !");
}

std::string Truck::getInfo()
{
    return "Truck - 18 wheels - heavy vehicle";
}

}  // namespace vehicle_plugins

PLUGINLIB_EXPORT_CLASS(vehicle_plugins::Truck, vehicle_base::VehicleBase)