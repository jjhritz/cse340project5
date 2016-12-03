cmake_minimum_required(VERSION 3.6)
project(cse340project5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES compiler.c compiler.h project5.cpp project5.h statement.h statement.cpp if.h if.cpp switch.h switch.cpp)
add_executable(cse340project5 ${SOURCE_FILES})