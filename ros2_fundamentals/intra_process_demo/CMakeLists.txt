cmake_minimum_required(VERSION 3.8)

project(intra_process_demo)

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
find_package(rclcpp_components REQUIRED)
find_package(std_msgs REQUIRED)

add_library(intra_process_demo SHARED
  src/producer_node.cpp
  src/consumer_node.cpp
)

target_compile_features(
  intra_process_demo
  PUBLIC
  cxx_std_17
)

ament_target_dependencies(
  intra_process_demo
  rclcpp
  rclcpp_components
  std_msgs
)

rclcpp_components_register_nodes(
  intra_process_demo
  "ProducerNode"
  "ConsumerNode"
)

install(
  TARGETS intra_process_demo
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(
  FILES intra_process_demo.xml
  DESTINATION share/${PROJECT_NAME}
)

install(
  DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()