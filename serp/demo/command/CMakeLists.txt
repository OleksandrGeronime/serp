cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)

add_executable(demo_command demo_command.cpp)
target_link_libraries(demo_command PRIVATE serp)
