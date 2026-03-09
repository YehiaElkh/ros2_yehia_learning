cmake_minimum_required(VERSION 3.8)
project(publisher_and_subscriber)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(custom_msg_and_srv REQUIRED)

add_executable(simple_publisher src/simple_publisher.cpp)
ament_target_dependencies(simple_publisher rclcpp std_msgs)

add_executable(simple_subscriber src/simple_subscriber.cpp)
ament_target_dependencies(simple_subscriber rclcpp std_msgs)
add_executable(publisher_with_custom_msg src/publisher_with_custom_msg.cpp)
ament_target_dependencies(publisher_with_custom_msg rclcpp custom_msg_and_srv)
add_executable(subscriber_with_custom_msg src/subscriber_with_custom_msg.cpp)
ament_target_dependencies(subscriber_with_custom_msg rclcpp custom_msg_and_srv)

install(TARGETS
  simple_publisher
  simple_subscriber
  publisher_with_custom_msg
  subscriber_with_custom_msg
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
