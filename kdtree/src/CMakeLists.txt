cmake_minimum_required(VERSION 3.2.2)

project(src)

add_definitions(-std=c++11)

set(HEADER_FILES
        Segment.h
        )

set(SOURCE_FILES
        DimensionSelector.h SegmentSplitter.h NodeBuilder.h KDTree.h KDTree.cpp KDTreeIO.h Util.h SplitGenerator.h)

add_library(src STATIC ${SOURCE_FILES} ${HEADER_FILES})
