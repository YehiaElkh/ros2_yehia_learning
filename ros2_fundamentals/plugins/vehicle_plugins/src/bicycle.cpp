#include "vehicle_plugins/bicycle.hpp"
#include "pluginlib/class_list_macros.hpp"

namespace vehicle_plugins
{

void Bicycle::move()
{
    RCLCPP_INFO(rclcpp::get_logger("Bicycle"), "Bicycle is moving smoothly ! 🚲");
}

void Bicycle::stop()
{
    RCLCPP_INFO(rclcpp::get_logger("Bicycle"), "Bicycle stopped !");
}

std::string Bicycle::getInfo()
{
    return "Bicycle - 2 wheels - eco friendly vehicle";
}

}  // namespace vehicle_plugins

PLUGINLIB_EXPORT_CLASS(vehicle_plugins::Bicycle, vehicle_base::VehicleBase)