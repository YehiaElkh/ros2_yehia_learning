cmake_minimum_required(VERSION 3.8)

project(lifecycle_manager_demo)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(lifecycle_msgs REQUIRED)

add_executable(perception_lifecycle_node
  src/perception_lifecycle_node.cpp
)

ament_target_dependencies(
  perception_lifecycle_node
  rclcpp
  rclcpp_lifecycle
)

add_executable(planning_lifecycle_node
  src/planning_lifecycle_node.cpp
)

ament_target_dependencies(
  planning_lifecycle_node
  rclcpp
  rclcpp_lifecycle
)

add_executable(control_lifecycle_node
  src/control_lifecycle_node.cpp
)

ament_target_dependencies(
  control_lifecycle_node
  rclcpp
  rclcpp_lifecycle
)

add_executable(lifecycle_manager
  src/lifecycle_manager.cpp
)

ament_target_dependencies(
  lifecycle_manager
  rclcpp
  lifecycle_msgs
)

install(
  TARGETS
    perception_lifecycle_node
    planning_lifecycle_node
    control_lifecycle_node
    lifecycle_manager
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()