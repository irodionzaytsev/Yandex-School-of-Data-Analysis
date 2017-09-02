cmake_minimum_required(VERSION 3.6)
project(Contest5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Duche.h Cyclic.h Directors.h SecretAgents.h MinTree.h)
add_executable(Contest5 ${SOURCE_FILES})