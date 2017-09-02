cmake_minimum_required(VERSION 3.3)
project(YandexCpp4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES test.cpp A.h readers_util.h readers.h test-2.cpp unique_ptr.h)
add_executable(YandexCpp4 ${SOURCE_FILES})