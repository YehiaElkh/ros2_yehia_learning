cmake_minimum_required(VERSION 3.8)
project(dynamic_tf2_publisher)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2_ros REQUIRED)

add_executable(dynamic_tf2_publisher src/dynamic_tf2_publisher.cpp)
ament_target_dependencies(dynamic_tf2_publisher
  rclcpp
  geometry_msgs
  tf2_ros
)

install(TARGETS
  dynamic_tf2_publisher
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()