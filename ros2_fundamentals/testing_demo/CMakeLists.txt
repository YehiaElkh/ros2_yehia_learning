cmake_minimum_required(VERSION 3.8)

project(testing_demo)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(ament_cmake_gtest REQUIRED)

add_library(calculator
  src/calculator.cpp
)

target_include_directories(
  calculator PUBLIC
  include
)

ament_add_gtest(test_calculator
  test/test_calculator.cpp
)

target_link_libraries(
  test_calculator
  calculator
)

ament_target_dependencies(
  test_calculator
)

ament_package()