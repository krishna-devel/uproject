include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runTests
        SegmentTest.cpp
        MockTest.cpp
        SplitDimensionSelectorTest.cpp
        KDTreeTest.cpp
        KDTreeExplorerTest.cpp
        NodeBuilderTest.cpp
        KDTreeIOTest.cpp
        UtilTest.cpp
        SplitPointSelectorTest.cpp
        EndToEndTest.cpp )

target_link_libraries(runTests gmock gtest gtest_main)
target_link_libraries(runTests src)
target_link_libraries(runTests tbb)

find_package( Boost COMPONENTS system filesystem iostreams REQUIRED )
if ( Boost_FOUND )
    include_directories( ${Boost_INCLUDE_DIRS} )
    target_link_libraries(runTests ${Boost_LIBRARIES} )
endif( Boost_FOUND )