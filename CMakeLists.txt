# CMake config
cmake_minimum_required(VERSION 2.6)

project (itcFramework)

message ("PROJECT_NAME: ${PROJECT_NAME}")

if (NOT CMAKE_INSTALL_INCLUDEDIR  AND NOT DEFINED ENV{OECORE_TARGET_SYSROOT} )
    message ("PC build, not SDK")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPCBUILD")
endif (NOT CMAKE_INSTALL_INCLUDEDIR  AND NOT DEFINED ENV{OECORE_TARGET_SYSROOT})

set (BT_WARNING_SUPPRESSION_FLAGS "-Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-variable -Wno-unused-value")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11 -Wno-variadic-macros ${BT_WARNING_SUPPRESSION_FLAGS}")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${BT_WARNING_SUPPRESSION_FLAGS}")

set(BT_ROOT_DIR "${CMAKE_SOURCE_DIR}")

if(NOT DEFINED COMMON_API_DIR)
    set(COMMON_API_DIR "${CMAKE_SOURCE_DIR}/common-api")
endif()

message ("CMAKE_CURRENT_SOURCE_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")
message ("bt root dir : ${CMAKE_CURRENT_SOURCE_DIR}")
message ("common api dir : ${COMMON_API_DIR}")
message ("common api include dir : ${COMMON_API_INCLUDE_DIR}")
message ("common api src dir : ${COMMON_API_SRC_DIR}")
message ("bt build dir : ${BT_BUILD_DIRECTORY}")

# Common includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/_private)

#add sources
set(SRC_LIB_FILES
    _private/Dispatcher.cpp
    _private/Event.cpp
    _private/EventLoop.cpp
    _private/itc.cpp
    _private/Timer.cpp
)

set(SRC_EXE_FILES
    main.cpp
)

# Make .so
add_library(${PROJECT_NAME} SHARED  ${SRC_LIB_FILES} )

# Make executable file
add_executable("main" ${SRC_EXE_FILES})

target_link_libraries(
    "main"
    ${PROJECT_NAME}
    pthread
)

