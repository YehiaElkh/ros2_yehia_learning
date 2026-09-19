cmake_minimum_required(VERSION 3.8)

project(sensor_fusion_demo)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(object_fusion
  src/object_fusion.cpp
)

ament_target_dependencies(
  object_fusion
  rclcpp
  std_msgs
)

install(
  TARGETS
    object_fusion
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()