cmake_minimum_required(VERSION 3.23)
project(SimpleLog)

set(CMAKE_CXX_STANDARD 14)
include_directories(.)

add_library(SimpleLogger src/Logger.cpp src/LogStream.cpp
            src/AsyncLogWriter1.cpp src/AsyncLogWriter2.cpp src/AsyncLogWriter3.cpp)
add_library(Utils utils/CountDownLatch.cpp utils/Buffer.cpp)
add_executable(Entry main.cpp)
target_link_libraries(Entry SimpleLogger Utils)

add_subdirectory(lib/googletest)
add_executable(testSingleThread ut/testSingleThread.cpp)
add_executable(testLogWriter1 ut/testLogWriter1.cpp)
add_executable(testLogWriter2 ut/testLogWriter2.cpp)
add_executable(testLogWriter3 ut/testLogWriter3.cpp)
target_link_libraries(testSingleThread SimpleLogger Utils gtest_main)
target_link_libraries(testLogWriter1 SimpleLogger Utils gtest_main)
target_link_libraries(testLogWriter2 SimpleLogger Utils gtest_main)
target_link_libraries(testLogWriter3 SimpleLogger Utils gtest_main)
