cmake_minimum_required(VERSION 3.6)
project(base)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    anillo.h
    tests.cpp)

add_executable(base ${SOURCE_FILES})