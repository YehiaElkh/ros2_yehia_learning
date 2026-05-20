
cmake_minimum_required(VERSION 3.8)
project(amr_controller)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(rclpy REQUIRED)
find_package(nav2_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)

# --------------------------------------------------------
# C++ exécutables
# --------------------------------------------------------
add_executable(goal_sender src/goal_sender.cpp)
ament_target_dependencies(goal_sender
  rclcpp rclcpp_action nav2_msgs geometry_msgs)

add_executable(waypoint_sender src/waypoint_sender.cpp)
ament_target_dependencies(waypoint_sender
  rclcpp rclcpp_action nav2_msgs geometry_msgs)

install(TARGETS
  goal_sender
  waypoint_sender
  DESTINATION lib/${PROJECT_NAME}
)

# --------------------------------------------------------
# Python module
# --------------------------------------------------------
ament_python_install_package(${PROJECT_NAME})

# Installer le script Python comme exécutable
install(PROGRAMS
  amr_controller/mission_planner.py
  DESTINATION lib/${PROJECT_NAME}
  RENAME mission_planner
)

# --------------------------------------------------------
# Resources
# --------------------------------------------------------
install(DIRECTORY
  launch
  config
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
