#include "gtest/gtest.h"
#include "DimensionSelector.h"

class CycleThroughAxesDimensionSelectorTest : public testing::Test {
protected:
    virtual void SetUp() {
        samples <<
                1.0, 2.0, 3.0,
                4.0, 5.0, 6.0,
                7.0, 8.0, 9.0;
    }
    Samples<float> samples {3, 3};
};

class HighestRangeAxisDimensionSelectorTest : public testing::Test {
protected:
    virtual void SetUp() {
        samples1 <<
                1.0, 9.0, 3.0,
                4.0, 5.0, 6.0,
                7.0, 1.0, 9.0;

        samples2 <<
                1.0, 2.0, 3.0,
                4.0, 5.0, 7.0,
                7.0, 8.0, 9.0;
    }
    Samples<float> samples1 {3, 3};
    Samples<float> samples2 {3, 3};
};

TEST_F(CycleThroughAxesDimensionSelectorTest, getNextDimensionToSplit_first_dimension) {
    Segment<float, int> segment (samples);
    CycleThroughAxesDimensionSelector<float, int> dimensionSelector (-1);
    DimensionWithSplitInfo<float, int> dimensionWithSplitInfo =
            dimensionSelector.getNextDimensionToSplit(segment, DimensionSplittingMethod::MEDIAN_OF_MEDIAN);
    ASSERT_EQ(0, dimensionWithSplitInfo.getSplitDimension());
    ASSERT_EQ(4.0, dimensionWithSplitInfo.getSplitInfo().getThreshold());
}

TEST_F(CycleThroughAxesDimensionSelectorTest, getNextDimensionToSplit_dimension_in_between) {
    Segment<float, int> segment (samples);
    CycleThroughAxesDimensionSelector<float, int> dimensionSelector (1);
    DimensionWithSplitInfo<float, int> dimensionWithSplitInfo =
            dimensionSelector.getNextDimensionToSplit(segment, DimensionSplittingMethod::MEDIAN_OF_MEDIAN);
    ASSERT_EQ(2, dimensionWithSplitInfo.getSplitDimension());
    ASSERT_EQ(6.0, dimensionWithSplitInfo.getSplitInfo().getThreshold());
}

TEST_F(CycleThroughAxesDimensionSelectorTest, getNextDimensionToSplit_dimension_rotate_back_to_first_dimension) {
    Segment<float, int> segment (samples);
    CycleThroughAxesDimensionSelector<float, int> dimensionSelector (2);
    DimensionWithSplitInfo<float, int> dimensionWithSplitInfo =
            dimensionSelector.getNextDimensionToSplit(segment, DimensionSplittingMethod::MEDIAN_OF_MEDIAN);
    ASSERT_EQ(0, dimensionWithSplitInfo.getSplitDimension());
    ASSERT_EQ(4.0, dimensionWithSplitInfo.getSplitInfo().getThreshold());
}

TEST(DimensionWithSplitInfoTest, test_string_serilization) {
    SplitInfo<float> splitInfo (5.0);
    DimensionWithSplitInfo<float, int> dimensionWithSplitInfo1 (1, splitInfo);
    DimensionWithSplitInfo<float, int> dimensionWithSplitInfo2 =
            DimensionWithSplitInfo<float, int>::fromString(dimensionWithSplitInfo1.toString());
    ASSERT_EQ(5.0, dimensionWithSplitInfo2.getSplitInfo().getThreshold());
    ASSERT_EQ(1, dimensionWithSplitInfo2.getSplitDimension());
}

TEST_F(HighestRangeAxisDimensionSelectorTest, test_split_works_correctly_on_entire_segment) {
    Segment<float, int> segment (samples1);

    CycleThroughAxesDimensionSelector<float, int> cycleThroughAxesDimensionSelector (-1);
    DimensionWithSplitInfo<float, int> dimensionWithSplitInfo =
            cycleThroughAxesDimensionSelector.getNextDimensionToSplit(
                    segment,
                    DimensionSplittingMethod::MEDIAN_OF_MEDIAN
            );
    ASSERT_EQ(0, dimensionWithSplitInfo.getSplitDimension());
    ASSERT_EQ(4.0, dimensionWithSplitInfo.getSplitInfo().getThreshold());

    HighestRangeAxisDimensionSelector<float, int> highestRangeAxisDimensionSelector;
    DimensionWithSplitInfo<float, int> dimensionWithSplitInfo2 =
            highestRangeAxisDimensionSelector.getNextDimensionToSplit(
                    segment,
                    DimensionSplittingMethod::MEDIAN_OF_MEDIAN
            );
    ASSERT_EQ(1, dimensionWithSplitInfo2.getSplitDimension());
    ASSERT_EQ(5.0, dimensionWithSplitInfo2.getSplitInfo().getThreshold());
}

TEST_F(HighestRangeAxisDimensionSelectorTest, test_split_works_correctly_on_part_of_segments) {
    HighestRangeAxisDimensionSelector<float, int> highestRangeAxisDimensionSelector;

    vector<int> samplesInSegment1 {0, 1};
    Segment<float, int> segment1 (samples2, samplesInSegment1);
    DimensionWithSplitInfo<float, int> dimensionWithSplitInfo1 =
            highestRangeAxisDimensionSelector.getNextDimensionToSplit(
                    segment1,
                    DimensionSplittingMethod::MEDIAN_OF_MEDIAN
            );
    ASSERT_EQ(2, dimensionWithSplitInfo1.getSplitDimension());
    ASSERT_EQ(5.0, dimensionWithSplitInfo1.getSplitInfo().getThreshold());

    vector<int> samplesInSegment2 {1, 2};
    Segment<float, int> segment2 (samples2, samplesInSegment2);
    DimensionWithSplitInfo<float, int> dimensionWithSplitInfo2 =
            highestRangeAxisDimensionSelector.getNextDimensionToSplit(
                    segment2,
                    DimensionSplittingMethod::MEDIAN_OF_MEDIAN
            );
    ASSERT_EQ(0, dimensionWithSplitInfo2.getSplitDimension());
    ASSERT_EQ(5.5, dimensionWithSplitInfo2.getSplitInfo().getThreshold());
}
