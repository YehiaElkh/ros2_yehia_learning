cmake_minimum_required(VERSION 3.8)

project(service_client_demo)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(example_interfaces REQUIRED)

add_executable(add_two_ints_server
  src/add_two_ints_server.cpp
)

ament_target_dependencies(add_two_ints_server
  rclcpp
  example_interfaces
)

add_executable(add_two_ints_client
  src/add_two_ints_client.cpp
)

ament_target_dependencies(add_two_ints_client
  rclcpp
  example_interfaces
)

install(
  TARGETS
    add_two_ints_server
    add_two_ints_client
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()