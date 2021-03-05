cmake_minimum_required(VERSION 3.17)
project(Manual_Testing)

set(CMAKE_CXX_STANDARD 14)

# Test Object Include
include_directories(../Test_Object)

# Implementation Include Data Structures
include_directories(${CMAKE_SOURCE_DIR}/Data_Structures/)

# Test Cases
add_executable(Manual_Testing Manual_Testing.cpp)
