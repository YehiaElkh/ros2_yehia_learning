cmake_minimum_required(VERSION 3.8)
project(start_with_simple_nodes)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Dépendances
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

# Exécutable
add_executable(my_first_node src/my_first_node.cpp)
ament_target_dependencies(my_first_node rclcpp)
add_executable(node_with_timer src/node_with_timer.cpp)
ament_target_dependencies(node_with_timer rclcpp)

# Installation
install(TARGETS
  my_first_node
  node_with_timer
  DESTINATION lib/${PROJECT_NAME}
)


ament_package()