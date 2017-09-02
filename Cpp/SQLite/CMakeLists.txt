cmake_minimum_required(VERSION 3.6)
project(SQLite)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp sqlwrapper.h test.cpp)
add_executable(SQLite ${SOURCE_FILES})