cmake_minimum_required(VERSION 3.10)
project(MP2_ARITHMETIC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_subdirectory(gtest)

add_subdirectory(src)

add_subdirectory(test)

add_subdirectory(samples)