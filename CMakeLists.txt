cmake_minimum_required(VERSION 3.15)
project(BigNumber)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(BigNumber BigNumber.hpp main.cpp)
