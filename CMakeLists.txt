cmake_minimum_required(VERSION 2.6)
project(Pregolya)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wno-c++11-extensions")

add_subdirectory(test)
