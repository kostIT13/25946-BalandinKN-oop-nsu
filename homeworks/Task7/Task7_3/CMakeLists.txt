cmake_minimum_required(VERSION 3.16)
project(QuadrupedRobot7_3 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(robot_book main_book.cpp robot.cpp)
add_executable(robot_containers main_containers.cpp robot.cpp)
