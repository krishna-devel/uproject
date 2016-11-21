cmake_minimum_required(VERSION 3.2.2)

project(src)

add_definitions(-std=c++11)

set(HEADER_FILES
        MatrixXdAdapter.h
        )

set(SOURCE_FILES
        MatrixXdAdapter.cpp
        Axis.h
        AxisScorer.h
        Util.h
        )

#include_directories(../include)

add_library(src STATIC ${SOURCE_FILES} ${HEADER_FILES})
#add_library(src STATIC ${HEADER_FILES})
