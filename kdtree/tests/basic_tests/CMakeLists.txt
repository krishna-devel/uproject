include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runTests
        SegmentTest.cpp
        MockTest.cpp
        DimensionSelectorTest.cpp
        SegmentSplitterTest.cpp KDTreeTest.cpp NodeBuilderTest.cpp KDTreeIOTest.cpp UtilTest.cpp SplitGeneratorTest.cpp)

#add_subdirectory(mocks)

target_link_libraries(runTests gmock gtest gtest_main)
target_link_libraries(runTests src)

find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    target_link_libraries(runTests ${ZLIB_LIBRARIES} )
endif( ZLIB_FOUND )

find_package( Boost COMPONENTS system filesystem iostreams REQUIRED )
if ( Boost_FOUND )
    include_directories( ${Boost_INCLUDE_DIRS} )
    target_link_libraries(runTests ${Boost_LIBRARIES} )
endif( Boost_FOUND )