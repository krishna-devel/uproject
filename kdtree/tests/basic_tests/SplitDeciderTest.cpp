#include "gtest/gtest.h"
#include "SplitDecider.h"

class SplitDeciderTest : public testing::Test {
protected:
    virtual void SetUp() {
        samplesWithOneRow << 1.0, 2.0, 3.0, 4.0, 5.0;
        samplesWithMultipleRows <<
                1.0, 2.0, 3.0, 4.0, 5.0,
                6.0, 7.0, 8.0, 9.0, 10.0,
                11.0, 12.0, 13.0, 14.0, 15.0,
                16.0, 17.0, 18.0, 19.0, 20.0,
                21.0, 22.0, 23.0, 24.0, 25.0,
                26.0, 27.0, 28.0, 29.0, 30.0;
    }
    Samples<float> samplesWithNoRows;
    Samples<float> samplesWithOneRow {1, 5};
    Samples<float> samplesWithMultipleRows {6, 5};
};

TEST_F(SplitDeciderTest, shouldSplit_samplesWithMultipleRows) {
    Segment<float, int> segment (samplesWithMultipleRows);
    bool shouldSplit = SplitDecider<float, int>::shouldSplit(segment);
    ASSERT_EQ(true, shouldSplit);
}

TEST_F(SplitDeciderTest, shouldSplit_samplesWithOneRow) {
    Segment<float, int> segment (samplesWithOneRow);
    bool shouldSplit = SplitDecider<float, int>::shouldSplit(segment);
    ASSERT_EQ(false, shouldSplit);
}

TEST_F(SplitDeciderTest, shouldSplit_samplesWithNoRows) {
    Segment<float, int> segment (samplesWithNoRows);
    bool shouldSplit = SplitDecider<float, int>::shouldSplit(segment);
    ASSERT_EQ(false, shouldSplit);
}


