cmake_minimum_required(VERSION 3.8)

project(composition_demo)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(std_msgs REQUIRED)


add_library(camera_component SHARED
  src/camera_component.cpp
)

ament_target_dependencies(
  camera_component
  rclcpp
  rclcpp_components
  std_msgs
)


rclcpp_components_register_nodes(
  camera_component
  "CameraComponent"
)


add_library(perception_component SHARED
  src/perception_component.cpp
)

ament_target_dependencies(
  perception_component
  rclcpp
  rclcpp_components
  std_msgs
)


rclcpp_components_register_nodes(
  perception_component
  "PerceptionComponent"
)


install(
  TARGETS
    camera_component
    perception_component
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)


ament_package()