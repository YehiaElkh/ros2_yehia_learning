cmake_minimum_required(VERSION 3.8)

project(thread_safety_demo)

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
find_package(std_msgs REQUIRED)

add_executable(
  thread_safety_node
  src/thread_safety_node.cpp
)

ament_target_dependencies(
  thread_safety_node
  rclcpp
  std_msgs
)

install(
  TARGETS
    thread_safety_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()