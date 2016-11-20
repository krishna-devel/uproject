include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runTests
        basic_check.cpp
        MatrixXdAdapterTest.cpp
        )

target_link_libraries(runTests gtest gtest_main)
target_link_libraries(runTests src)
