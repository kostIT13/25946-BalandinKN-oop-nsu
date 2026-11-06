add_library(ordinary_lib INTERFACE)
target_include_directories(ordinary_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(ordinary_lib INTERFACE cxx_std_17)
export(TARGETS ordinary_lib NAMESPACE ordinary_lib:: FILE ordinary_libConfig.cmake)