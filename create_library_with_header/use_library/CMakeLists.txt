cmake_minimum_required(VERSION 3.8)
project(use_library)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(publisher_library REQUIRED)

add_executable(use_library src/use_library.cpp)

ament_target_dependencies(use_library rclcpp)

target_link_libraries(use_library
  publisher_library::publisher_library
)

install(TARGETS use_library
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()