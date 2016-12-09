cmake_minimum_required(VERSION 3.2.2)

project(src)

add_definitions(-std=c++11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(SOURCE_FILES KDTree.cpp)

set(BUILD_KDTREE_SOURCE_FILES
        BuildKDTree.cpp)

set(QUERY_KDTREE_SOURCE_FILES
        QueryKDTree.cpp)

set(HEADER_FILES
        Segment.h
        SplitDimensionSelector.h
        KDTreeBuilder.h
        KDTree.h
        KDTreeIO.h
        Util.h
        SplitPointSelector.h
        KDTreeExplorer.h)

add_executable(build_kdtree ${BUILD_KDTREE_SOURCE_FILES})
add_executable(query_kdtree ${QUERY_KDTREE_SOURCE_FILES})

find_package( Boost COMPONENTS system filesystem iostreams program_options REQUIRED )
if ( Boost_FOUND )
    include_directories( ${Boost_INCLUDE_DIRS} )
    target_link_libraries(build_kdtree ${Boost_LIBRARIES} )
    target_link_libraries(query_kdtree ${Boost_LIBRARIES} )
endif( Boost_FOUND )

add_library(src STATIC ${SOURCE_FILES} ${HEADER_FILES})
