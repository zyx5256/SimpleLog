cmake_minimum_required(VERSION 3.23)
project(SimpleLog)

set(CMAKE_CXX_STANDARD 14)

add_library(SimpleLogger src/Log.cpp)
add_executable(Entry main.cpp)
target_link_libraries(Entry SimpleLogger)

add_subdirectory(lib/googletest)
add_executable(TestLog ut/LogTest.cpp)
target_link_libraries(TestLog Utils gtest_main)
