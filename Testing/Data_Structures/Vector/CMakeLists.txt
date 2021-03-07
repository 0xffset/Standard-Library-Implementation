cmake_minimum_required(VERSION 3.17)
project(Vector_Test)

set(CMAKE_CXX_STANDARD 14)

add_executable(Vector_Test Vector_Test.cpp)
target_link_libraries(Vector_Test ${Boost_LIBRARIES})

