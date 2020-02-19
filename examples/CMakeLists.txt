cmake_minimum_required(VERSION 3.12)

include(add_example.cmake)

add_example(basic
    TEST
    TEST --help
    TEST 1 2 --flag test)

