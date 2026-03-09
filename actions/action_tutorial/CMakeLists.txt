cmake_minimum_required(VERSION 3.8)
project(action_tutorial)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(custom_msg_and_srv REQUIRED)

add_executable(action_server src/action_server.cpp)
ament_target_dependencies(action_server rclcpp rclcpp_action custom_msg_and_srv)

add_executable(action_client src/action_client.cpp)
ament_target_dependencies(action_client rclcpp rclcpp_action custom_msg_and_srv)

install(TARGETS
  action_server
  action_client
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()