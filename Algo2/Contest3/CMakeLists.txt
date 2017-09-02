cmake_minimum_required(VERSION 3.6)
project(Contest3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp A.h AA.h B.h BB.h BBB.h)
add_executable(Contest3 ${SOURCE_FILES})