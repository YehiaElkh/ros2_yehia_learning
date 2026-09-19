cmake_minimum_required(VERSION 3.8)

project(sensor_processing_demo)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)

add_executable(camera_processor
  src/camera_processor.cpp
)

ament_target_dependencies(
  camera_processor
  rclcpp
  sensor_msgs
)

add_executable(lidar_processor
  src/lidar_processor.cpp
)

ament_target_dependencies(
  lidar_processor
  rclcpp
  sensor_msgs
)

install(
  TARGETS
    camera_processor
    lidar_processor
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()