cmake_minimum_required(VERSION 3.16)
project(QuadrupedRobotTask11_1 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(robot
    main.cpp
    robot.cpp
    cargo_robot.cpp
    autonomous_robot.cpp
    cargo_autonomous_robot.cpp
    diamond_problem.cpp)

if(MSVC)
    target_compile_options(robot PRIVATE /W4)
else()
    target_compile_options(robot PRIVATE -Wall -Wextra)
endif()
