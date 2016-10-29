cmake_minimum_required(VERSION 3.5)
project(SeedCup)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES SeedCup.cpp)
add_executable(SeedCup ${SOURCE_FILES})