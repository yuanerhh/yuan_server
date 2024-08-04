cmake_minimum_required(VERSION 3.0)
project(yuan_server)

file(GLOB SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
set(LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")

add_library(yuan_server SHARED ${SRC_LIST})

target_compile_options(yuan_server PRIVATE -g -ggdb -DDEBUG -fsanitize=address)
target_link_libraries(yuan_server PRIVATE -fsanitize=address)
