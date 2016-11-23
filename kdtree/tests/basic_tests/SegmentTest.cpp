#include <gtest/gtest.h>
#include "Segment.h"

class SegmentTest : public testing::Test {
protected:
    virtual void SetUp() {
        fSamples <<
                1.f,2.f,
                3.f,4.f;
        dSamples <<
                1.0,2.0,
                3.0,4.0,
                5.0,6.0;
    }

    Samples<float> fSamples {2, 2};
    Samples<double> dSamples {3, 2};

};

TEST_F(SegmentTest, check_indices_are_correct_if_initialized_without_constructor) {
    Segment<float, int> segment (fSamples);
    vector<int> expectedIndices {0,1};

    ASSERT_EQ(expectedIndices, segment.getSampleIdsInSegment());
    ASSERT_EQ(fSamples.rows(), segment.getSampleIdsInSegment().size());
}

TEST_F(SegmentTest, check_indices_are_correct_if_initialized_with_constructor) {
    vector<long> passedIndices {0,2};
    Segment<double, long> segment (dSamples, passedIndices);

    vector<long> expectedIndices {0,2};
    ASSERT_EQ(expectedIndices, segment.getSampleIdsInSegment());
    ASSERT_EQ(fSamples.rows(), segment.getSampleIdsInSegment().size());
}
