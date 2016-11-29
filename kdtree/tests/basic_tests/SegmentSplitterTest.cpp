#include "gtest/gtest.h"
#include "SegmentSplitter.h"
#include <stdlib.h>
#include "memory"

using namespace std;

class SegmentSplitterTest : public testing::Test {
protected:
    virtual void SetUp() {
        segmentSplitter = unique_ptr<SegmentSplitter<float, int>> (new SegmentSplitter<float, int>());
        samplesWithOneRow << 1.0, 2.0, 3.0, 4.0, 5.0;
        samplesWithMultipleRows <<
                1.0, 2.0, 3.0, 4.0, 5.0,
                6.0, 7.0, 8.0, 9.0, 10.0,
                11.0, 12.0, 13.0, 14.0, 15.0,
                16.0, 17.0, 18.0, 19.0, 20.0,
                21.0, 22.0, 23.0, 24.0, 25.0,
                26.0, 27.0, 28.0, 29.0, 30.0;
        samplesWithDuplicateRows <<
                               1.0, 2.0, 3.0,
                               1.0, 2.0, 3.0,
                               1.0, 2.0, 3.0;

    }
    static bool isBalanced(const SplitSegments<float, int> &splitSegments) {
        long diff = labs(splitSegments.getSegmentLessThanThreshold().getSampleIdsInSegment().size() -
                     splitSegments.getSegmentGreaterThanThreshold().getSampleIdsInSegment().size()) <= 1;
        return diff;
    }
    Samples<float> samplesWithDuplicateRows {3, 3};
    Samples<float> samplesWithOneRow {1, 5};
    Samples<float> samplesWithMultipleRows {6, 5};
    unique_ptr<SegmentSplitter<float, int>> segmentSplitter;
};

TEST_F(SegmentSplitterTest, split_base_case) {
    Segment<float, int> segment (samplesWithMultipleRows);

    vector<float> splitPointVector {11.0, 12.0, 13.0, 14.0, 15.0};
    Point<float, int> splitPoint(splitPointVector);

    SplitSegments<float, int> splitSegments = segmentSplitter->split(segment, splitPoint, 2);

    SampleIdsInSegment<int> allSegmentIds {0,1,2,3,4,5};
    SampleIdsInSegment<int> expectedSegmentIdsInLessThan {0,1,2};
    SampleIdsInSegment<int> expectedSegmentIdsInGreaterThan {3,4,5};

    ASSERT_EQ(segment.getSamples(), splitSegments.getSegmentLessThanThreshold().getSamples());
    ASSERT_EQ(segment.getSamples(), splitSegments.getSegmentGreaterThanThreshold().getSamples());

    ASSERT_EQ(allSegmentIds, segment.getSampleIdsInSegment());
    ASSERT_EQ(expectedSegmentIdsInLessThan, splitSegments.getSegmentLessThanThreshold().getSampleIdsInSegment());
    ASSERT_EQ(expectedSegmentIdsInGreaterThan, splitSegments.getSegmentGreaterThanThreshold().getSampleIdsInSegment());
    EXPECT_TRUE(SegmentSplitterTest::isBalanced(splitSegments));
}

TEST_F(SegmentSplitterTest, split_sample_with_one_row) {
    Segment<float, int> segment (samplesWithOneRow);

    vector<float> splitPointVector {1.0, 2.0, 3.0, 4.0, 5.0};
    Point<float, int> splitPoint(splitPointVector);

    SplitSegments<float, int> splitSegments = segmentSplitter->split(segment, splitPoint, 2);

    SampleIdsInSegment<int> allSegmentIds {0};
    SampleIdsInSegment<int> expectedSegmentIdsInLessThan {0};
    SampleIdsInSegment<int> expectedSegmentIdsInGreaterThan;

    ASSERT_EQ(segment.getSamples(), splitSegments.getSegmentLessThanThreshold().getSamples());
    ASSERT_EQ(segment.getSamples(), splitSegments.getSegmentGreaterThanThreshold().getSamples());

    ASSERT_EQ(allSegmentIds, segment.getSampleIdsInSegment());
    ASSERT_EQ(expectedSegmentIdsInLessThan, splitSegments.getSegmentLessThanThreshold().getSampleIdsInSegment());
    ASSERT_EQ(expectedSegmentIdsInGreaterThan, splitSegments.getSegmentGreaterThanThreshold().getSampleIdsInSegment());
    EXPECT_TRUE(SegmentSplitterTest::isBalanced(splitSegments));
}

TEST_F(SegmentSplitterTest, split_sample_with_duplicate_rows) {
    Segment<float, int> segment (samplesWithDuplicateRows);

    vector<float> splitPointVector {1.0, 2.0, 3.0};
    Point<float, int> splitPoint(splitPointVector);

    SplitSegments<float, int> splitSegments = segmentSplitter->split(segment, splitPoint, 2);
    SampleIdsInSegment<int> allSegmentIds {0, 1, 2};
    SampleIdsInSegment<int> expectedSegmentIdsInLessThan {0, 2};
    SampleIdsInSegment<int> expectedSegmentIdsInGreaterThan {1};

    ASSERT_EQ(segment.getSamples(), splitSegments.getSegmentLessThanThreshold().getSamples());
    ASSERT_EQ(segment.getSamples(), splitSegments.getSegmentGreaterThanThreshold().getSamples());

    ASSERT_EQ(allSegmentIds, segment.getSampleIdsInSegment());
    ASSERT_EQ(expectedSegmentIdsInLessThan, splitSegments.getSegmentLessThanThreshold().getSampleIdsInSegment());
    ASSERT_EQ(expectedSegmentIdsInGreaterThan, splitSegments.getSegmentGreaterThanThreshold().getSampleIdsInSegment());
    EXPECT_TRUE(SegmentSplitterTest::isBalanced(splitSegments));
}