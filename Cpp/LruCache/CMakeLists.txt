cmake_minimum_required(VERSION 3.3)
project(LruCache)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES test.cpp lru_cache.h)
add_executable(LruCache ${SOURCE_FILES})