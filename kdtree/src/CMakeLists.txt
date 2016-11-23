cmake_minimum_required(VERSION 3.2.2)

project(src)

add_definitions(-std=c++11)

set(HEADER_FILES
        Segment.h
        )

set(SOURCE_FILES
        Dimension.h
        DimensionSplitter.h
        Util.h
        Util.cpp Node.h DimensionSelector.h)

add_library(src STATIC ${SOURCE_FILES} ${HEADER_FILES})
