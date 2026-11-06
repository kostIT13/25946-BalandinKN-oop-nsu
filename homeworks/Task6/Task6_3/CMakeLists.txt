cmake_minimum_required(VERSION 3.16)
project(QuadrupedRobot CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Основная программа
add_executable(robot main.cpp robot.cpp)

# Тесты (googletest через FetchContent)
enable_testing()
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)

# Для Windows: чтобы gtest не перезаписывал настройки компилятора
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(robot_tests tests.cpp robot.cpp)
target_link_libraries(robot_tests PRIVATE GTest::gtest GTest::gtest_main)

add_test(NAME robot_tests COMMAND robot_tests)
