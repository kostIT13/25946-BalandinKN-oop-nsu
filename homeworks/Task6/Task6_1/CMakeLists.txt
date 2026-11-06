cmake_minimum_required(VERSION 3.16)
project(QuadrupedRobot CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Основная программа (задание 6.I: конструкторы и оператор присваивания)
add_executable(robot main.cpp robot.cpp)
