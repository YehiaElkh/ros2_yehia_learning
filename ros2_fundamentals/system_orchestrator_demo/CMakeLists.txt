cmake_minimum_required(VERSION 3.8)

project(system_orchestrator_demo)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

add_executable(system_orchestrator
  src/system_orchestrator.cpp
)

ament_target_dependencies(
  system_orchestrator
  rclcpp
)

install(
  TARGETS
    system_orchestrator
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()