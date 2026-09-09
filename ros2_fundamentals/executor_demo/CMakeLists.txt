cmake_minimum_required(VERSION 3.8)

project(executor_demo)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)


add_executable(executor_node
  src/executor_node.cpp
)


ament_target_dependencies(
  executor_node
  rclcpp
  std_msgs
)


install(
  TARGETS
    executor_node
  DESTINATION lib/${PROJECT_NAME}
)


ament_package()