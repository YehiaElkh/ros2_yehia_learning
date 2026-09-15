cmake_minimum_required(VERSION 3.8)

project(action_scheduler_demo)

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

add_executable(
  scheduler_server
  src/scheduler_server.cpp
)

ament_target_dependencies(
  scheduler_server
  rclcpp
  rclcpp_action
  example_interfaces
)

add_executable(
  scheduler_client
  src/scheduler_client.cpp
)

ament_target_dependencies(
  scheduler_client
  rclcpp
  rclcpp_action
  example_interfaces
)

install(
  TARGETS
    scheduler_server
    scheduler_client
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()