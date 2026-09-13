cmake_minimum_required(VERSION 3.8)

project(performance_tracing_demo)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
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
  camera_node
  src/camera_node.cpp
)

ament_target_dependencies(
  camera_node
  rclcpp
  std_msgs
)

add_executable(
  processing_node
  src/processing_node.cpp
)

ament_target_dependencies(
  processing_node
  rclcpp
  std_msgs
)

install(
  TARGETS
    camera_node
    processing_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()