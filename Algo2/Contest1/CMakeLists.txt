cmake_minimum_required(VERSION 3.6)
project(Contest1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp A.h B.h C.h D.h E.h)
add_executable(Contest1 ${SOURCE_FILES})