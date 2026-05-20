cmake_minimum_required(VERSION 3.8)
project(publisher_library)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

# Créer la shared library
add_library(publisher_library SHARED
  src/timed_publisher.cpp
)

target_include_directories(publisher_library PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(publisher_library rclcpp std_msgs)

install(TARGETS publisher_library
  EXPORT publisher_library_targets
  DESTINATION lib
)

install(DIRECTORY include/
  DESTINATION include
)

ament_export_targets(publisher_library_targets HAS_LIBRARY_TARGET)
ament_export_dependencies(rclcpp std_msgs)

ament_package()