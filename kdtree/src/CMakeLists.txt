cmake_minimum_required(VERSION 3.2.2)

project(src)

add_definitions(-std=c++11)

set(HEADER_FILES
        EigenMatrixAdapter.h
        )

set(SOURCE_FILES
        EigenMatrixAdapter.cpp
        Axis.h
        AxisSeparation.h
        Util.h
        Util.cpp)

add_library(src STATIC ${SOURCE_FILES} ${HEADER_FILES})
