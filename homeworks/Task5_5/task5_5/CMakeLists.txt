cmake_minimum_required(VERSION 3.16)
project(oop_hw LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build unit tests with GoogleTest" ON)

add_subdirectory(lib)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE ordinary_lib)

if(BUILD_TESTS)
    message(STATUS "Tests: ENABLED")

    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    add_subdirectory(tests)
else()
    message(STATUS "Tests: DISABLED")
endif()