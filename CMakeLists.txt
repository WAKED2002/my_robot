cmake_minimum_required(VERSION 3.8)
project(my_robot)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(turtlesim REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

add_executable(battery src/battery.cpp)
ament_target_dependencies(battery rclcpp std_msgs)

add_executable(battery_monitor src/battery_monitor.cpp)
ament_target_dependencies(battery_monitor rclcpp std_msgs)

add_executable(circle_driver src/circle_driver.cpp)
ament_target_dependencies(circle_driver rclcpp geometry_msgs)

add_executable(pose_reader src/pose_reader.cpp)
ament_target_dependencies(pose_reader rclcpp turtlesim)

add_executable(go_to_goal src/go_to_goal.cpp)
ament_target_dependencies(go_to_goal rclcpp turtlesim geometry_msgs)

add_executable(robot_go_to_goal src/robot_go_to_goal.cpp)
ament_target_dependencies(robot_go_to_goal rclcpp nav_msgs geometry_msgs)

add_executable(camera_reader src/camera_reader.cpp)
ament_target_dependencies(camera_reader rclcpp sensor_msgs)

add_executable(color_finder src/color_finder.cpp)
ament_target_dependencies(color_finder rclcpp sensor_msgs)

install(TARGETS battery battery_monitor circle_driver pose_reader go_to_goal robot_go_to_goal DESTINATION lib/${PROJECT_NAME})

install(TARGETS battery battery_monitor circle_driver pose_reader go_to_goal
        robot_go_to_goal camera_reader DESTINATION lib/${PROJECT_NAME})

install(TARGETS battery battery_monitor circle_driver pose_reader go_to_goal DESTINATION lib/${PROJECT_NAME})

install(TARGETS battery battery_monitor circle_driver pose_reader DESTINATION lib/${PROJECT_NAME})

install(TARGETS battery battery_monitor circle_driver DESTINATION lib/${PROJECT_NAME})
ament_package()

install(TARGETS battery battery_monitor circle_driver pose_reader go_to_goal
        robot_go_to_goal camera_reader color_finder DESTINATION lib/${PROJECT_NAME})