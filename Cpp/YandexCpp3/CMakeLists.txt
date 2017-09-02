cmake_minimum_required(VERSION 3.3)
project(YandexCpp3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp stack.h test.cpp static_map.h test_static_map.cpp ring_buffer.h test_ring_buffer.cpp test_ring_buffer.h dungeon.h test_dungeon.cpp example.cpp rogue.h)
add_executable(YandexCpp3 ${SOURCE_FILES})