include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runTests
        basic_check.cpp
        AxisSeparationTest.cpp
        AxisTest.cpp
        EigenMatrixAdapterTest.cpp
        UtilTest.cpp
        )

#add_subdirectory(mocks)

target_link_libraries(runTests gmock gtest gtest_main)
target_link_libraries(runTests src)
