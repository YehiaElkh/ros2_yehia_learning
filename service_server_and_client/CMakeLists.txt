cmake_minimum_required(VERSION 3.8)
project(service_server_and_client)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(custom_msg_and_srv REQUIRED)

add_executable(simple_server src/simple_server.cpp)
ament_target_dependencies(simple_server rclcpp custom_msg_and_srv)

add_executable(simple_client src/simple_client.cpp)
ament_target_dependencies(simple_client rclcpp custom_msg_and_srv)

install(TARGETS
  simple_server
  simple_client
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()