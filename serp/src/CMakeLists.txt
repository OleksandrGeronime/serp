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
    core/PTimer.cpp
    core/Service.cpp
    core/Timer.cpp
    core/Watchdog.cpp

    logger/Dumpers.cpp
    logger/LogManager.cpp
    logger/LogTag.cpp
    logger/Logger.cpp
    logger/LogWriter.cpp
    logger/types.cpp
    logger/strategies/LogStrategyCommon.cpp
    logger/strategies/LogStrategyConsole.cpp
    logger/strategies/LogStrategyFile.cpp

    command/Command.cpp
    command/CommandPool.cpp
    command/CommandProcessor.cpp
    command/CommandQueue.cpp
    command/ICommand.cpp
    command/LambdaCommand.cpp
)

# Create static/shared library
add_library(${PROJECT_NAME} STATIC ${SERP_SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC pthread)

# Public headers include paths
target_include_directories(${PROJECT_NAME}
    PUBLIC ${SERP_INCLUDE_DIRS}
)
