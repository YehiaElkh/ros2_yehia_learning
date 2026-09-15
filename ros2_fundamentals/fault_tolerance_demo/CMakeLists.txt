cmake_minimum_required(VERSION 3.8)

project(fault_tolerance_demo)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(monitored_node
  src/monitored_node.cpp
)

ament_target_dependencies(
  monitored_node
  rclcpp
  std_msgs
)

add_executable(fault_monitor
  src/fault_monitor.cpp
)

ament_target_dependencies(
  fault_monitor
  rclcpp
  std_msgs
)

install(
  TARGETS
    monitored_node
    fault_monitor
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()