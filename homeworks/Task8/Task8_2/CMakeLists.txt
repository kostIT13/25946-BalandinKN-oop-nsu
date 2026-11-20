cmake_minimum_required(VERSION 3.16)
project(QuadrupedRobot8_2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Приложение части (демо перемещения), собирается вместе с тестами
add_executable(robot main.cpp robot.cpp)

# ---------- Тесты: googletest + fixture (TEST_F) ----------
enable_testing()
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)

# Для Windows: чтобы gtest не перезаписывал настройки компилятора
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Чтобы не качать gtest заново, можно указать на уже скачанный исходник,
# например:
#   cmake -S . -B build -DFETCHCONTENT_SOURCE_DIR_GOOGLETEST=<path>/googletest-src
FetchContent_MakeAvailable(googletest)

add_executable(robot_tests tests.cpp robot.cpp)
target_link_libraries(robot_tests PRIVATE GTest::gtest GTest::gtest_main)

add_test(NAME robot_tests COMMAND robot_tests)

if(MSVC)
    target_compile_options(robot PRIVATE /W4)
    target_compile_options(robot_tests PRIVATE /W4)
else()
    target_compile_options(robot PRIVATE -Wall -Wextra)
    target_compile_options(robot_tests PRIVATE -Wall -Wextra)
endif()
