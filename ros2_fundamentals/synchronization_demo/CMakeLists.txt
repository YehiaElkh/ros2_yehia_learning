cmake_minimum_required(VERSION 3.8)

project(synchronization_demo)

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

add_executable(
  producer_consumer_node
  src/producer_consumer_node.cpp
)

ament_target_dependencies(
  producer_consumer_node
  rclcpp
)

install(
  TARGETS
    producer_consumer_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()