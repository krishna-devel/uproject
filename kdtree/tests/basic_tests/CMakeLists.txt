include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runTests
        basic_check.cpp
        AxisTest.cpp
        MatrixXdAdapterTest.cpp
        UtilTest.cpp)

target_link_libraries(runTests gtest gtest_main)
target_link_libraries(runTests src)
