cmake_minimum_required(VERSION 3.3)
project(NonIntrusiveList)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp intrusive_list.h intrusive_list_stolen.h)
add_executable(NonIntrusiveList ${SOURCE_FILES})