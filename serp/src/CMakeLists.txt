cmake_minimum_required(VERSION 3.22)
project(serp LANGUAGES CXX)

# Use modern C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
set(SERP_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOOST_INCLUDE_DIR}  # Optional: if used, ensure BOOST_INCLUDE_DIR is defined externally
)

# Source files
set(SERP_SOURCES
    core/App.cpp
    core/EventLoop.cpp
    core/Timer.cpp
    core/PTimer.cpp
    core/Watchdog.cpp
    core/Service.cpp
)

# Create static/shared library
add_library(${PROJECT_NAME} STATIC ${SERP_SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC pthread)

# Public headers include paths
target_include_directories(${PROJECT_NAME}
    PUBLIC ${SERP_INCLUDE_DIRS}
)
