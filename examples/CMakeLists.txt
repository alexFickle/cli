cmake_minimum_required(VERSION 3.12)

include(add_example.cmake)

add_example(basic
    TEST
    TEST --help
    TEST --usage
    TEST --version
    TEST 1 2 --bool --flag test)

