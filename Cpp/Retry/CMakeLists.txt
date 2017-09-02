cmake_minimum_required(VERSION 3.6)
project(Retry)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp LogicLayer.h Matrix.h retry.h)
add_executable(Retry ${SOURCE_FILES})