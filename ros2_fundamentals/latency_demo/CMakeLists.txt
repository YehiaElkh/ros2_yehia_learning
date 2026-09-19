cmake_minimum_required(VERSION 3.8)

project(latency_demo)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(latency_node
  src/latency_node.cpp
)

ament_target_dependencies(
  latency_node
  rclcpp
  std_msgs
)

install(
  TARGETS
    latency_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()