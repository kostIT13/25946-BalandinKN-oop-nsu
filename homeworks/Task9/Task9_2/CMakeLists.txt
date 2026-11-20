cmake_minimum_required(VERSION 3.16)
project(QuadrupedRobot9_2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(robot main.cpp robot.cpp admin.cpp)

if(MSVC)
    target_compile_options(robot PRIVATE /W4)
else()
    target_compile_options(robot PRIVATE -Wall -Wextra)
endif()
