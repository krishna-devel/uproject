include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runTests
        DimensionSplitterTest.cpp
        DimensionTest.cpp
        SegmentTest.cpp
        MockTest.cpp DimensionSelectorTest.cpp SegmentSplitterTest.cpp SplitDeciderTest.cpp KDTreeTest.cpp NodeBuilderTest.cpp)

#add_subdirectory(mocks)

target_link_libraries(runTests gmock gtest gtest_main)
target_link_libraries(runTests src)
