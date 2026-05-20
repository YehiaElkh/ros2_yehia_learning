#include "vehicle_plugins/motorbike.hpp"
#include "pluginlib/class_list_macros.hpp"
//       | macro pour enregistrer le plugin

namespace vehicle_plugins
{

void Motorbike::move()
{
    RCLCPP_INFO(rclcpp::get_logger("Motorbike"), "Motorbike is moving fast ! 🏍️");
}

void Motorbike::stop()
{
    RCLCPP_INFO(rclcpp::get_logger("Motorbike"), "Motorbike stopped !");
}

std::string Motorbike::getInfo()
{
    return "Motorbike - 2 wheels - fast vehicle";
}

}  // namespace vehicle_plugins

// ============================================================
// ENREGISTRER LE PLUGIN
// ============================================================
// PLUGINLIB_EXPORT_CLASS(classe_concrete, classe_de_base)
// Cette macro est OBLIGATOIRE pour que pluginlib trouve le plugin
PLUGINLIB_EXPORT_CLASS(vehicle_plugins::Motorbike, vehicle_base::VehicleBase)