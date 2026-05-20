cmake_minimum_required(VERSION 3.8)
project(message_sync)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(message_filters REQUIRED)

add_executable(multi_publisher src/multi_publisher.cpp)
ament_target_dependencies(multi_publisher rclcpp std_msgs)

add_executable(sync_subscriber src/sync_subscriber.cpp)
ament_target_dependencies(sync_subscriber rclcpp std_msgs message_filters)

install(TARGETS
  multi_publisher
  sync_subscriber
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()