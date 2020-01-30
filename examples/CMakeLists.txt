cmake_minimum_required(VERSION 3.10)

function(add_example NAME)
    add_executable(${NAME}_example ${NAME}_example.cpp)
    target_link_libraries(${NAME}_example cli)
endfunction()

add_example(fake)
