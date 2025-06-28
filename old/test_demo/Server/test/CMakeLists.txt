# CMake config
cmake_minimum_required(VERSION 2.6)

project (test_itctestdemo)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

message ("PROJECT_NAME: ${PROJECT_NAME}")

if (NOT CMAKE_INSTALL_INCLUDEDIR  AND NOT DEFINED ENV{OECORE_TARGET_SYSROOT} )
    message ("PC build, not SDK")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPCBUILD")
endif (NOT CMAKE_INSTALL_INCLUDEDIR  AND NOT DEFINED ENV{OECORE_TARGET_SYSROOT})

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../../" "${BUILD_DIRECTORY}/itcFramework")

set (WARNING_SUPPRESSION_FLAGS "-Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-variable -Wno-unused-value")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11 -Wno-variadic-macros ${WARNING_SUPPRESSION_FLAGS}")

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
message ("BUILD_DIRECTORY : ${BUILD_DIRECTORY}")
message ("ITC_ROOT_DIR : ${ITC_ROOT_DIR}")

# Common includes
include_directories(
    ${ITC_ROOT_DIR} 
    ${GTEST_INCLUDE_DIRS}
)

#add sources
set(SRC_FILES
    main.cpp
    ServerTest.cpp
    ../../../itctest/Validator.cpp
    ../private/Server.cpp
)

# Make executable file
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(
    ${PROJECT_NAME}
    pthread
    itcframework
    ${GTEST_LIBRARIES}
)