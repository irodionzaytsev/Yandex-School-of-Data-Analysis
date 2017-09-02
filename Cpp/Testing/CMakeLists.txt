cmake_minimum_required(VERSION 3.3)
project(Testing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES test.cpp near_set.h gtest/)
add_executable(Testing ${SOURCE_FILES})