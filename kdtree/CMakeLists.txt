cmake_minimum_required(VERSION 3.2.2)
project(kdtree)

add_definitions(-std=c++11)

set(SOURCE_FILES main.cpp)
add_executable(kdtree_run ${SOURCE_FILES})

include_directories(src)
include_directories(include)
include_directories(lib/tbb/include/)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(lib/tbb)

find_package( Boost COMPONENTS system filesystem iostreams program_options REQUIRED )
if ( Boost_FOUND )
    include_directories( ${Boost_INCLUDE_DIRS} )
    target_link_libraries(kdtree_run ${Boost_LIBRARIES} )
endif( Boost_FOUND )

target_link_libraries(kdtree_run tbb)
