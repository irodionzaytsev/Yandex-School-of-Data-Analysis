cmake_minimum_required(VERSION 3.6)
project(Contest2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp Aho_Corasick.h coraseck_send.h)
add_executable(Contest2 ${SOURCE_FILES})