include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runTests
        basic_check.cpp
        DimensionSplitterTest.cpp
        DimensionTest.cpp
        SegmentTest.cpp
        UtilTest.cpp
        NodeTest.cpp
        MockTest.cpp DimensionSelectorTest.cpp SegmentSplitterTest.cpp SplitDeciderTest.cpp KDTreeTest.cpp)

#add_subdirectory(mocks)

target_link_libraries(runTests gmock gtest gtest_main)
target_link_libraries(runTests src)
