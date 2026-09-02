cmake_minimum_required(VERSION 3.8)

project(pub_sub_demo)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(publisher_node
  src/publisher_node.cpp
)

ament_target_dependencies(publisher_node
  rclcpp
  std_msgs
)

add_executable(subscriber_node
  src/subscriber_node.cpp
)

ament_target_dependencies(subscriber_node
  rclcpp
  std_msgs
)

install(
  TARGETS
    publisher_node
    subscriber_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
