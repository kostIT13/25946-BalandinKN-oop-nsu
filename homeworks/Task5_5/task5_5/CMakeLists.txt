cmake_minimum_required(VERSION 3.16)
project(oop_hw LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing() # CTest генерирует тесты только при вызове в корневом CMakeLists.

add_subdirectory(lib)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE ordinary_lib)

add_subdirectory(tests)
