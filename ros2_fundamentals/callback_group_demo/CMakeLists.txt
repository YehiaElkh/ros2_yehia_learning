cmake_minimum_required(VERSION 3.8)

project(callback_group_demo)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(callback_group_node
  src/callback_group_node.cpp
)

ament_target_dependencies(
  callback_group_node
  rclcpp
  std_msgs
)

install(
  TARGETS
    callback_group_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()