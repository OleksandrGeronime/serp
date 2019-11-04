# CMake config
cmake_minimum_required(VERSION 2.6)

project (itcframework)

message ("PROJECT_NAME: ${PROJECT_NAME}")

if (NOT CMAKE_INSTALL_INCLUDEDIR  AND NOT DEFINED ENV{OECORE_TARGET_SYSROOT} )
    message ("PC build, not SDK")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPCBUILD")
endif (NOT CMAKE_INSTALL_INCLUDEDIR  AND NOT DEFINED ENV{OECORE_TARGET_SYSROOT})

if (NOT WARNING_SUPPRESSION_FLAGS)
    set (WARNING_SUPPRESSION_FLAGS "-Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-variable -Wno-unused-value")
endif (NOT WARNING_SUPPRESSION_FLAGS)

if (NOT CMAKE_CXX_FLAGS)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11 -Wno-variadic-macros ${WARNING_SUPPRESSION_FLAGS}")
endif (NOT CMAKE_CXX_FLAGS)

set(ITC_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

message ("ITC_ROOT_DIR: ${ITC_ROOT_DIR}")
message ("BUILD_DIRECTORY: ${BUILD_DIRECTORY}")

# Common includes
include_directories(${ITC_ROOT_DIR})

#add sources
set(SRC_FILES
    _private/Dispatcher.cpp
    _private/Event.cpp
    _private/EventLoop.cpp
    _private/itc.cpp
    _private/Timer.cpp
    _private/Logger.cpp
)

# Make .so
add_library(${PROJECT_NAME} SHARED  ${SRC_FILES} )

target_link_libraries(
    ${PROJECT_NAME}
    pthread
)

