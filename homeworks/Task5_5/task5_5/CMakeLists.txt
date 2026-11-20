cmake_minimum_required(VERSION 3.16)
project(oop_hw LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build unit tests with GoogleTest" ON)

add_subdirectory(lib)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE ordinary_lib)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()