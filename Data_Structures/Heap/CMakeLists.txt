cmake_minimum_required(VERSION 3.17)
project(Heap)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR}/Data_Structures/)

add_library(Heap Heap.cpp Heap.hpp)