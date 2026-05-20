cmake_minimum_required(VERSION 3.8)
project(vehicle_base)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(pluginlib REQUIRED)

# Exécutable principal (vehicle loader)
add_executable(create_vehicle src/vehicle_loader.cpp)
ament_target_dependencies(create_vehicle rclcpp pluginlib)

# Inclure les headers
target_include_directories(create_vehicle PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

install(TARGETS create_vehicle
  DESTINATION lib/${PROJECT_NAME}
)

# Installer les headers pour que vehicle_plugins puisse les utiliser
install(DIRECTORY include/
  DESTINATION include
)

ament_export_include_directories(include)
ament_export_dependencies(rclcpp pluginlib)

ament_package()