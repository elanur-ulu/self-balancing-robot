cmake_minimum_required(VERSION 3.8)
project(balance_bot_control)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Bağımlılıklar
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)

# C++ Düğümünü Derleme
add_executable(balance_node src/balance_node.cpp)
ament_target_dependencies(balance_node rclcpp geometry_msgs sensor_msgs std_msgs)

# 1. C++ Düğümünü Sisteme Yükleme (TARGETS)
install(TARGETS
  balance_node
  DESTINATION lib/${PROJECT_NAME}
)

# 2. URDF Klasörünü Sisteme Yükleme (DIRECTORY)
install(DIRECTORY
  urdf
  DESTINATION share/${PROJECT_NAME}
)

# En sonda olmalı
ament_package()