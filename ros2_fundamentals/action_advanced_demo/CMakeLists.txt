cmake_minimum_required(VERSION 3.8)

project(action_advanced_demo)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
  )
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(advanced_action_server
  src/advanced_action_server.cpp
)

ament_target_dependencies(
  advanced_action_server
  rclcpp
  rclcpp_action
  example_interfaces
)

add_executable(advanced_action_client
  src/advanced_action_client.cpp
)

ament_target_dependencies(
  advanced_action_client
  rclcpp
  rclcpp_action
  example_interfaces
)

install(
  TARGETS
    advanced_action_server
    advanced_action_client
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()