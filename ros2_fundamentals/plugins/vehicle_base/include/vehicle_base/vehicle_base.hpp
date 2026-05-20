#ifndef VEHICLE_BASE__VEHICLE_BASE_HPP_
#define VEHICLE_BASE__VEHICLE_BASE_HPP_

#include <string>        // ← ajouter cet include !
#include "rclcpp/rclcpp.hpp"

namespace vehicle_base
{

class VehicleBase
{
public:
    virtual ~VehicleBase() = default;

    virtual void move() = 0;
    virtual void stop() = 0;
    virtual std::string getInfo() = 0;

protected:
    virtual void initialize() {}
};

}  // namespace vehicle_base

#endif  // VEHICLE_BASE__VEHICLE_BASE_HPP_