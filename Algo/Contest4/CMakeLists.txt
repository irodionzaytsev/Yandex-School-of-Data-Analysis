cmake_minimum_required(VERSION 3.3)
project(Contest3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp TreeOrder.h AliBaba.h AliBaba2.h Kingdoms.h CountTrees.h FixedSet.h CountStream.h Relatives.h)
add_executable(Contest3 ${SOURCE_FILES})