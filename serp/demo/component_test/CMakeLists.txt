cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)

add_executable(demo_test CUT.cpp DemoTest.cpp TestCases.cpp DependencyMock.cpp)
target_link_libraries(demo_test PRIVATE serp gtest gtest_main)