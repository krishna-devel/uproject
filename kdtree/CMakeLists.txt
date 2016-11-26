cmake_minimum_required(VERSION 3.2.2)
project(kdtree)

add_definitions(-std=c++11)

set(SOURCE_FILES main.cpp)
add_executable(kdtree_run ${SOURCE_FILES})

include_directories(src)
include_directories(include)

add_subdirectory(src)
add_subdirectory(tests)

find_package (Threads)
target_link_libraries(kdtree_run ${CMAKE_THREAD_LIBS_INIT})


#if ($ENV{CLION_IDE})
#    message("Call cmake command from the CLion IDE")
#    set(CALENDAR_PATH_VAR "test")
#    set(CALENDAR_DIR_VAR "test")
#endif ()