cmake_minimum_required(VERSION 3.3)
project(YandexCpp2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp A.h B.h H.h C.h E.h F.h G.h D.h)
add_executable(YandexCpp2 ${SOURCE_FILES})