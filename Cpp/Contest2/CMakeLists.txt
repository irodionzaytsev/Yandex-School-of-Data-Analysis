cmake_minimum_required(VERSION 3.3)
project(Contest2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp heap.h)
add_executable(Contest2 ${SOURCE_FILES})