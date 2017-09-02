cmake_minimum_required(VERSION 3.3)
project(Lambdas)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp removefindif.h unique.h reverse_copy.h any_of.h set_difference.h)
add_executable(Lambdas ${SOURCE_FILES})