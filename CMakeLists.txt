cmake_minimum_required(VERSION 3.10)

project(cli CXX)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include(CTest)

add_library(cli INTERFACE)
target_include_directories(cli INTERFACE include)
target_compile_features(cli INTERFACE cxx_std_17)

if(BUILD_TESTING)
    add_subdirectory(test)
    add_subdirectory(examples)
endif()
