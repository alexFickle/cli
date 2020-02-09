cmake_minimum_required(VERSION 3.10)

include(GoogleTest)

add_executable(test_cli
    arity_test.cpp
    array_traits_test.cpp
    command_line_test.cpp
    destination_test.cpp
    parse_test.cpp
)
target_link_libraries(test_cli PRIVATE cli ${CONAN_LIBS})

gtest_discover_tests(test_cli NO_PRETTY_TYPES)
