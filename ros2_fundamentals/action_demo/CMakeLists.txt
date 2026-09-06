cmake_minimum_required(VERSION 3.8)

project(action_demo)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(example_interfaces REQUIRED)

add_executable(fibonacci_action_server
  src/fibonacci_action_server.cpp
)

ament_target_dependencies(
  fibonacci_action_server
  rclcpp
  rclcpp_action
  example_interfaces
)

add_executable(fibonacci_action_client
  src/fibonacci_action_client.cpp
)

ament_target_dependencies(
  fibonacci_action_client
  rclcpp
  rclcpp_action
  example_interfaces
)

install(
  TARGETS
    fibonacci_action_server
    fibonacci_action_client
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()