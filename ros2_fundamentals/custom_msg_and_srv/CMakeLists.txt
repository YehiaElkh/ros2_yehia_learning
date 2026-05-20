cmake_minimum_required(VERSION 3.8)
project(custom_msg_and_srv)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# Déclarer les fichiers de messages
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/EmployeeSalary.msg"
  "srv/AddTwoInts.srv"
  "action/Concatenate.action"
)

ament_package()
