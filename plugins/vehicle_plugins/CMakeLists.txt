cmake_minimum_required(VERSION 3.8)
project(vehicle_plugins)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(pluginlib REQUIRED)
find_package(vehicle_base REQUIRED)

# Créer une bibliothèque partagée (pas un exécutable !)
# Les plugins sont des .so chargés dynamiquement
add_library(vehicle_plugins SHARED
  src/motorbike.cpp
  src/bicycle.cpp
  src/truck.cpp
)

target_include_directories(vehicle_plugins PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(vehicle_plugins rclcpp pluginlib vehicle_base)

# Enregistrer les plugins via plugins.xml
pluginlib_export_plugin_description_file(vehicle_base plugins.xml)
#                                         ↑
#                          package de la classe de base

install(TARGETS vehicle_plugins
  DESTINATION lib
)

install(DIRECTORY include/
  DESTINATION include
)

ament_package()