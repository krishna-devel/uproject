#cmake_minimum_required(VERSION 3.2.2)

#project(basic_tests)

#add_definitions(-std=c++11)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runBasicTests
        basic_check.cpp
        MatrixXdAdapterTest.cpp
        )

#include_directories(../../include)
#include_directories(../../src)
#include_directories(../lib/gtest-1.7.0)

#add_subdirectory(../lib/gtest-1.7.0)

target_link_libraries(runBasicTests gtest gtest_main)
target_link_libraries(runBasicTests src)
