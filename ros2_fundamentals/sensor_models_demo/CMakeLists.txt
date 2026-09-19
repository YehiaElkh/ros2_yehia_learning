cmake_minimum_required(VERSION 3.8)

project(sensor_models_demo)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(geometry_msgs REQUIRED)

add_executable(sensor_projection_demo
  src/sensor_projection_demo.cpp
)

ament_target_dependencies(
  sensor_projection_demo
  rclcpp
  tf2
  tf2_ros
  geometry_msgs
)

install(
  TARGETS
    sensor_projection_demo
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()