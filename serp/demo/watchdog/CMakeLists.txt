cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)

add_executable(demo_watchdog demo_watchdog.cpp)
target_link_libraries(demo_watchdog PRIVATE serp)