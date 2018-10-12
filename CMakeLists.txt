cmake_minimum_required(VERSION 3.10)
project(OS_1)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98")

add_executable(OS_1 main.cpp util.h datastructures.h datastructures.cpp interface.cpp)