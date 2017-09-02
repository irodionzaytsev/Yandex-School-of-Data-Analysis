cmake_minimum_required(VERSION 3.3)
project(FirLess)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES FirLess/main.cpp FirLess/B.h FirLess/C.h FirLess/D.h FirLess/E.h FirLess/F.h FirLess/G.h FirLess/H.h SecLess/main.cpp SecLess/main.h)
add_executable(FirLess ${SOURCE_FILES})