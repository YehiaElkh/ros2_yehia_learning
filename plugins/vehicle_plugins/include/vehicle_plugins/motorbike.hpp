#ifndef VEHICLE_PLUGINS__MOTORBIKE_HPP_
#define VEHICLE_PLUGINS__MOTORBIKE_HPP_

#include "vehicle_base/vehicle_base.hpp"
#include "rclcpp/rclcpp.hpp"

namespace vehicle_plugins
{

class Motorbike : public vehicle_base::VehicleBase
{
public:
    void move()  override;
    void stop()  override;
    std::string getInfo() override;
};

}  // namespace vehicle_plugins

#endif