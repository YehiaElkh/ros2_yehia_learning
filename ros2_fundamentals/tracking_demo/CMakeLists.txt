cmake_minimum_required(VERSION 3.8)

project(tracking_demo)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(simple_tracker
  src/simple_tracker.cpp
)

ament_target_dependencies(
  simple_tracker
  rclcpp
  std_msgs
)

install(
  TARGETS
    simple_tracker
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()