#include <cmath>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "tf2/LinearMath/Matrix3x3.h"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2/LinearMath/Transform.h"
#include "tf2/LinearMath/Vector3.h"

class SensorProjectionDemo : public rclcpp::Node{
    public:
        SensorProjectionDemo() : Node("sensor_projection_demo"){
            // Camera intrinsics
            const double fx = 800.0;
            const double fy = 800.0;
            const double cx = 640.0;
            const double cy = 360.0;

            // Example 3D point in LiDAR frame
            tf2::Vector3 point_lidar(2.0, 0.0, 10.0);

            // LiDAR -> Camera transformation
            tf2::Quaternion q;
            q.setRPY(0.0, 0.0, 0.0);

            tf2::Vector3 translation(0.5, 0.0, 0.2);

            tf2::Transform T_camera_lidar;
            T_camera_lidar.setOrigin(translation);
            T_camera_lidar.setRotation(q);

            // Transform point
            tf2::Vector3 point_camera = T_camera_lidar * point_lidar;

            const double X = point_camera.x();
            const double Y = point_camera.y();
            const double Z = point_camera.z();

            if (Z <= 0.0){
            RCLCPP_WARN(
                get_logger(),
                "Point is behind the camera.");
            return;
            }

            // Project 3D -> image
            const double u = fx * X / Z + cx;
            const double v = fy * Y / Z + cy;

            RCLCPP_INFO(
            get_logger(),
            "LiDAR point: (%.2f, %.2f, %.2f)",
            point_lidar.x(),
            point_lidar.y(),
            point_lidar.z());

            RCLCPP_INFO(
            get_logger(),
            "Camera point: (%.2f, %.2f, %.2f)",
            X, Y, Z);

            RCLCPP_INFO(
            get_logger(),
            "Image pixel: (%.2f, %.2f)",
            u, v);
        }
};

int main(int argc, char ** argv){
  rclcpp::init(argc, argv);
  auto node = std::make_shared<SensorProjectionDemo>();
  rclcpp::spin_some(node);
  rclcpp::shutdown();
  return 0;
}