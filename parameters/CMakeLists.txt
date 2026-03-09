cmake_minimum_required(VERSION 3.8)
project(parameters)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

add_executable(node_with_params src/node_with_params.cpp)
ament_target_dependencies(node_with_params rclcpp)

install(TARGETS
  node_with_params
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()