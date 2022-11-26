cmake_minimum_required(VERSION 3.23)
project(SimpleLog)

set(CMAKE_CXX_STANDARD 14)
include_directories(.)

add_library(SimpleLogger src/Logger.cpp)
add_executable(Entry main.cpp)
target_link_libraries(Entry SimpleLogger)

add_subdirectory(lib/googletest)
add_executable(LogTest ut/LogTest.cpp)
target_link_libraries(LogTest SimpleLogger gtest_main)
