cmake_minimum_required(VERSION 3.8)

project(parameter_demo)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rcl_interfaces REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(configurable_node
  src/configurable_node.cpp
)

ament_target_dependencies(
  configurable_node
  rclcpp
  rcl_interfaces
  std_msgs
)

install(
  TARGETS
    configurable_node
  DESTINATION lib/${PROJECT_NAME}
)

install(
  DIRECTORY config
  DESTINATION share/${PROJECT_NAME}
)

ament_package()