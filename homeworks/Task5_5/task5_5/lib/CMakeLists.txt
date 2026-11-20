cmake_minimum_required(VERSION 3.16)
project(ordinary_lib_project LANGUAGES CXX)

add_library(ordinary_lib INTERFACE)

# Передаём потребителям путь к lib/ чтобы работал #include "average.h"
target_include_directories(ordinary_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Передаём потребителям требование C++17
target_compile_features(ordinary_lib INTERFACE cxx_std_17) 

# C:\Users\Yaroslav\Desktop\PROJECTS\1seminar\homeworks\Task5_5\task5_5\tests\average_test.cpp(2,10): error C1083: Не удается открыть файл включение: average.h: No s
# uch file or directory, [C:\Users\Yaroslav\Desktop\PROJECTS\1seminar\homeworks\task5_5\Task5_5\build\tests\lib_tests.vcxproj]