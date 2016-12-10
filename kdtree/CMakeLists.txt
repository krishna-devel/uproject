cmake_minimum_required(VERSION 3.2.2)
project(kdtree)

add_definitions(-std=c++11)

include_directories(src)
include_directories(lib/)
include_directories(lib/tbb/include/)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(lib/tbb)
