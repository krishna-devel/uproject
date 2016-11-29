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
    ASSERT_EQ(0, dimensionSelector.getNextDimension(segment));
}

TEST_F(CycleThroughAxesDimensionSelectorTest, getNextDimensionToSplit_dimension_in_between) {
    Segment<float, int> segment (samples);
    CycleThroughAxesDimensionSelector<float, int> dimensionSelector (1);
    ASSERT_EQ(2, dimensionSelector.getNextDimension(segment));
}

TEST_F(CycleThroughAxesDimensionSelectorTest, getNextDimensionToSplit_dimension_rotate_back_to_first_dimension) {
    Segment<float, int> segment (samples);
    CycleThroughAxesDimensionSelector<float, int> dimensionSelector (2);
    ASSERT_EQ(0, dimensionSelector.getNextDimension(segment));
}

TEST_F(HighestRangeAxisDimensionSelectorTest, test_split_works_correctly_on_entire_segment) {
    Segment<float, int> segment (samples1);

    CycleThroughAxesDimensionSelector<float, int> cycleThroughAxesDimensionSelector (-1);
    ASSERT_EQ(0, cycleThroughAxesDimensionSelector.getNextDimension(segment));

    HighestRangeAxisDimensionSelector<float, int> highestRangeAxisDimensionSelector;
    ASSERT_EQ(1, highestRangeAxisDimensionSelector.getNextDimension(segment));
}

TEST_F(HighestRangeAxisDimensionSelectorTest, test_split_works_correctly_on_part_of_segments) {
    HighestRangeAxisDimensionSelector<float, int> highestRangeAxisDimensionSelector;

    vector<int> samplesInSegment1 {0, 1};
    Segment<float, int> segment1 (samples2, samplesInSegment1);
    ASSERT_EQ(2, highestRangeAxisDimensionSelector.getNextDimension(segment1));

    vector<int> samplesInSegment2 {1, 2};
    Segment<float, int> segment2 (samples2, samplesInSegment2);
    ASSERT_EQ(0, highestRangeAxisDimensionSelector.getNextDimension(segment2));
}
