cmake_minimum_required(VERSION 3.0)
project(test_yuan_server)

include_directories(..)
link_directories(..)
add_executable(echo_server echo_server.cpp)
target_link_libraries(echo_server PRIVATE yuan_server)
target_compile_options(echo_server PRIVATE -g -ggdb -DDEBUG)
