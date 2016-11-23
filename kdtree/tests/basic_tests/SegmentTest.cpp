#include <gtest/gtest.h>
#include "Segment.h"

class SegmentTest : public testing::Test {
protected:
    virtual void SetUp() {
        fMatrix <<
                1.f,2.f,
                3.f,4.f;
        dMatrix <<
                1.0,2.0,
                3.0,4.0,
                5.0,6.0;
    }

    Matrix<float, Dynamic, Dynamic> fMatrix {2, 2};
    Matrix<double, Dynamic, Dynamic> dMatrix {3, 2};

};

TEST_F(SegmentTest, check_indices_are_correct_if_initialized_without_constructor) {
    Segment<float, int> segment (fMatrix);
    vector<int> expectedIndices {0,1};

    ASSERT_EQ(expectedIndices, segment.getSamplesInSegment());
    ASSERT_EQ(fMatrix.rows(), segment.getSamplesInSegment().size());
}

TEST_F(SegmentTest, check_indices_are_correct_if_initialized_with_constructor) {
    vector<long> passedIndices {0,2};
    Segment<double, long> segment (dMatrix, passedIndices);

    vector<long> expectedIndices {0,2};
    ASSERT_EQ(expectedIndices, segment.getSamplesInSegment());
    ASSERT_EQ(fMatrix.rows(), segment.getSamplesInSegment().size());
}
