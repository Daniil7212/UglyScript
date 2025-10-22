cmake_minimum_required(VERSION 3.31)
project(UglyScript)

set(CMAKE_CXX_STANDARD 20)

add_executable(UglyScript main.cpp)

configure_file(
    input.txt
    ${CMAKE_CURRENT_BINARY_DIR}/input.txt
    COPYONLY
)