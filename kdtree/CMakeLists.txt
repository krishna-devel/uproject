cmake_minimum_required(VERSION 3.2.2)
project(kdtree)

add_definitions(-std=c++11)

set(SOURCE_FILES main.cpp)
add_executable(kdtree_run ${SOURCE_FILES})

include_directories(src)
include_directories(include)
#include_directories(lib/tbb/include/)

add_subdirectory(src)
add_subdirectory(tests)
#add_subdirectory(lib/tbb)

find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    target_link_libraries(kdtree_run ${ZLIB_LIBRARIES} )
endif( ZLIB_FOUND )

find_package (Threads)
target_link_libraries(kdtree_run ${CMAKE_THREAD_LIBS_INIT})
#target_link_libraries(kdtree_run tbb)


#if ($ENV{CLION_IDE})
#    message("Call cmake command from the CLion IDE")
#    set(CALENDAR_PATH_VAR "test")
#    set(CALENDAR_DIR_VAR "test")
#endif ()