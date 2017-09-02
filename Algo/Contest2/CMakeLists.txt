cmake_minimum_required(VERSION 3.3)
project(Contest2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Heap.h Footballers.h MaxCommonSequence.h CoverCircle.h Coler.h Sort.h Sort2.h test.h Distances.h Distances2.h DistancesFourie.h)
add_executable(Contest2 ${SOURCE_FILES})