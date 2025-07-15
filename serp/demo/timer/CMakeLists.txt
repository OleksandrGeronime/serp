cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)

add_executable(demo_timer demo_timer.cpp)
target_link_libraries(demo_timer PRIVATE serp)