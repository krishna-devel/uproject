#include "gtest/gtest.h"
#include "SplitDimensionSelector.h"

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
    int nextDimension = DimensionSelectorByType<float, int>::get(segment, DimensionSelectorType::CYCLE_THROUGH_AXES, -1);
    ASSERT_EQ(0, nextDimension);
}

TEST_F(CycleThroughAxesDimensionSelectorTest, getNextDimensionToSplit_dimension_in_between) {
    Segment<float, int> segment (samples);
    int nextDimension = DimensionSelectorByType<float, int>::get(segment, DimensionSelectorType::CYCLE_THROUGH_AXES, 1);
    ASSERT_EQ(2, nextDimension);
}

TEST_F(CycleThroughAxesDimensionSelectorTest, getNextDimensionToSplit_dimension_rotate_back_to_first_dimension) {
    Segment<float, int> segment (samples);
    int nextDimension = DimensionSelectorByType<float, int>::get(segment, DimensionSelectorType::CYCLE_THROUGH_AXES, 2);
    ASSERT_EQ(0, nextDimension);
}

TEST_F(HighestRangeAxisDimensionSelectorTest, test_split_works_correctly_on_entire_segment) {
    Segment<float, int> segment (samples1);

    int nextDimension1 = DimensionSelectorByType<float, int>::get(segment, DimensionSelectorType::CYCLE_THROUGH_AXES, -1);
    ASSERT_EQ(0, nextDimension1);

    int nextDimension2 = DimensionSelectorByType<float, int>::get(segment, DimensionSelectorType::HIGHEST_RANGE_AXIS, -1);
    ASSERT_EQ(1, nextDimension2);
}

TEST_F(HighestRangeAxisDimensionSelectorTest, test_split_works_correctly_on_part_of_segments) {
    vector<int> samplesInSegment1 {0, 1};
    Segment<float, int> segment1 (samples2, samplesInSegment1);
    int nextDimension1 = DimensionSelectorByType<float, int>::get(segment1, DimensionSelectorType::HIGHEST_RANGE_AXIS, -1);
    ASSERT_EQ(2, nextDimension1);

    vector<int> samplesInSegment2 {1, 2};
    Segment<float, int> segment2 (samples2, samplesInSegment2);
    int nextDimension2 = DimensionSelectorByType<float, int>::get(segment2, DimensionSelectorType::HIGHEST_RANGE_AXIS, -1);
    ASSERT_EQ(0, nextDimension2);
}
