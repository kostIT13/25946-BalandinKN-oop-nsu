add_executable(lib_tests
    average_test.cpp
    mode_average_test.cpp
)

target_link_libraries(lib_tests
    PRIVATE
        ordinary_lib
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(lib_tests)