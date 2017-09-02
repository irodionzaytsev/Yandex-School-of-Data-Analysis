cmake_minimum_required(VERSION 3.3)
project(Contest1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp problemA.h problemB.h problemC.h problemCC.h RandomC.h problemCCC.h problemD.h seminar.h)
add_executable(Contest1 ${SOURCE_FILES})

