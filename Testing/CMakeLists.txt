cmake_minimum_required(VERSION 3.17)
project(Testing)

set(CMAKE_CXX_STANDARD 14)

# Boost
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(Boost_INCLUDE_DIRS C:\\LIBRARIES\\CPP\\Boost\\boost_1_75_0)
set(Boost_LIBRARIES C:\\LIBRARIES\\CPP\\Boost\\boost_1_75_0\\stage\\lib)

include_directories(${Boost_INCLUDE_DIRS})

# Test Object Include
include_directories(Test_Object)

# Implementation Include Data Structures
include_directories(${CMAKE_SOURCE_DIR}/Data_Structures/)

# Test Cases
add_subdirectory(Data_Structures/Array)
add_subdirectory(Data_Structures/Vector)
