cmake_minimum_required(VERSION 3.8)

project(realtime_demo)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(realtime_control_node
  src/realtime_control_node.cpp
)

ament_target_dependencies(
  realtime_control_node
  rclcpp
  std_msgs
)

install(
  TARGETS
    realtime_control_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()