cmake_minimum_required(VERSION 3.16)
project(QuadrupedRobotTask12_1 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(robot
    main.cpp
    robot.cpp
    quadruped_robot.cpp
    cargo_quadruped_robot.cpp
    autonomous_cargo_quadruped_robot.cpp)

if(MSVC)
    target_compile_options(robot PRIVATE /W4)
else()
    target_compile_options(robot PRIVATE -Wall -Wextra)
endif()
