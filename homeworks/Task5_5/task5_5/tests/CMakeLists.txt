option(BUILD_TESTS "Build unit tests with GoogleTest" ON)
if(NOT BUILD_TESTS)
    return()
endif()

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(lib_tests      # создаёт один тестовый бинарник из двух исходников
    average_test.cpp
    mode_average_test.cpp
)

target_link_libraries(lib_tests    # линкует lib_tests с двумя целями
    PRIVATE
        ordinary_lib
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(lib_tests)  # регистрирует каждый TEST() из lib_tests в CTest как отдельный тест
