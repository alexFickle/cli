cmake_minimum_required(VERSION 3.10)

project(cli CXX)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include(CTest)

add_library(cli
    src/cli.cpp
)
target_include_directories(cli PUBLIC include)
target_compile_features(cli PUBLIC cxx_std_17)
target_link_libraries(cli ${CONAN_LIBS})

if(BUILD_TESTING)
    add_subdirectory(test)
    add_subdirectory(examples)
endif()
