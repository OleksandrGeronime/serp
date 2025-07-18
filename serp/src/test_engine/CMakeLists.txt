cmake_minimum_required(VERSION 3.22)
project(test_engine LANGUAGES CXX)

set(TEST_ENGINE_SOURCES
    TestRunner.cpp
    ReplyHolder.cpp
)

add_library(${PROJECT_NAME} STATIC ${TEST_ENGINE_SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}
)
