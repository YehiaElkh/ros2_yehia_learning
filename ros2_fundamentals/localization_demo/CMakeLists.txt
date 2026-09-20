cmake_minimum_required(VERSION 3.8)

project(localization_demo)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

add_executable(localization_node
  src/localization_node.cpp
)

ament_target_dependencies(
  localization_node
  rclcpp
)

install(
  TARGETS
    localization_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()