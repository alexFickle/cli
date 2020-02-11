cmake_minimum_required(VERSION 3.12)

include(CTest)


#
# add_example(<NAME> [TEST [<ARG>]...]...)
#
# Creates an executable for an example and optionaly
# adds tests that run it.
#
# NAME: The name of the example source file (without the .cpp)
# TEST: Followed by command line arguments that will be passed
#       to the example exectuable.  If the executable returns
#       non-zero the test fails.  There may be multiple tests.
#
function(add_example NAME)
    # create the executable
    add_executable(${NAME}_cli_example ${NAME}.cpp)
    target_link_libraries(${NAME}_cli_example cli)

    # pop off the name argument
    set(args ${ARGV})
    list(REMOVE_AT args 0)
    
    # create all of the requested tests
    set(test_count 0)
    while(1)

        list(LENGTH args length)
        if(length EQUAL 0)
            return()
        endif()

        get_test_args(args test_args)

        add_test(NAME "cli/example/${NAME}/${test_count}"
            COMMAND ${NAME}_cli_example ${test_args})
        
        math(EXPR test_count "${test_count} + 1")

    endwhile()
endfunction()


#
# get_test_args(<LIST> <TEST_ARGS>)
#
# Helper function for add_example().  Extracts testing command
# line arguments from a list of potentially many tests.
# 
# LIST: The list containing [TEST [<AGRS>]...]...  Must not be empty.
#       Will have its first TEST [<ARGS>...] removed.
# TEST_ARGS: Output parameter for the arguments of the first test.
#
function(get_test_args LIST TEST_ARGS)
    # validate the input list
    list(LENGTH ${LIST} length)
    if(length EQUAL 0)
        message(FATAL_ERROR "Internal error in add_example.")
    endif()
    list(GET ${LIST} 0 must_be_test)
    if(NOT (must_be_test STREQUAL "TEST"))
        message(FATAL_ERROR
            "Interal error or bad arguments to add_example.")
    endif()

    # pop off the leading "TEST"
    set(args ${${LIST}})
    list(REMOVE_AT args 0)

    # find the start of the next test, if one exists
    list(FIND args "TEST" next_test_index)
    if(next_test_index EQUAL -1)
        # this is the last test, all args are for this test
        set(${TEST_ARGS} ${args} PARENT_SCOPE)
        set(${LIST} "" PARENT_SCOPE)
    else()
        # there are more tests, split off this one's args
        list(SUBLIST args 0 ${next_test_index} test_args)
        set(${TEST_ARGS} ${test_args} PARENT_SCOPE)
        list(SUBLIST args ${next_test_index} -1 list)
        set(${LIST} ${list} PARENT_SCOPE)
    endif()

endfunction()
