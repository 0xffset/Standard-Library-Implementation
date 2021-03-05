cmake_minimum_required(VERSION 3.17)
project(Cpp_std_lib)

set(CMAKE_CXX_STANDARD 14)

# Implementations
add_subdirectory(Data_Structures/Array)
add_subdirectory(Data_Structures/Disjoint_Set)
add_subdirectory(Data_Structures/Heap)
add_subdirectory(Data_Structures/Vector)

# Testing
add_subdirectory(Testing)

# Manual Testing
add_subdirectory(Testing/Manual_Testing)
