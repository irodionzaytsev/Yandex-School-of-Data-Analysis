cmake_minimum_required(VERSION 3.3)
project(Seminars)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp seminar1subsegments.h seminar2topk.h seminar2topkdirty.h seminar4squares.h seminar8counting.h seminar7segments.h seminar7segments_arrays.h seminar7segments_fenwick.h)
add_executable(Seminars ${SOURCE_FILES})