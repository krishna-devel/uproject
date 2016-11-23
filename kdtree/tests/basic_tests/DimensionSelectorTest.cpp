#include "gtest/gtest.h"
#include "DimensionSelector.h"

class LoopingDimensionSelectorTest : public testing::Test {
protected:
    virtual void SetUp() {
        samples <<
                1.0, 2.0, 3.0,
                4.0, 5.0, 6.0,
                7.0, 8.0, 9.0;
    }
    Samples<float> samples {3, 3};
};

TEST_F(LoopingDimensionSelectorTest, getNextDimensionToSplit_first_dimension) {
    Segment<float, int> segment (samples);
    LoopingDimensionSelector<float, int> dimensionSelector (-1);
    DimensionWithSplitInfo<float> dimensionWithSplitInfo = dimensionSelector.getNextDimensionToSplit(segment);
    ASSERT_EQ(0, dimensionWithSplitInfo.getSplitDimension());
    ASSERT_EQ(4.0, dimensionWithSplitInfo.getSplitInfo().getThreshold());
}

TEST_F(LoopingDimensionSelectorTest, getNextDimensionToSplit_dimension_in_between) {
    Segment<float, int> segment (samples);
    LoopingDimensionSelector<float, int> dimensionSelector (1);
    DimensionWithSplitInfo<float> dimensionWithSplitInfo = dimensionSelector.getNextDimensionToSplit(segment);
    ASSERT_EQ(2, dimensionWithSplitInfo.getSplitDimension());
    ASSERT_EQ(6.0, dimensionWithSplitInfo.getSplitInfo().getThreshold());
}

TEST_F(LoopingDimensionSelectorTest, getNextDimensionToSplit_dimension_rotate_back_to_first_dimension) {
    Segment<float, int> segment (samples);
    LoopingDimensionSelector<float, int> dimensionSelector (2);
    DimensionWithSplitInfo<float> dimensionWithSplitInfo = dimensionSelector.getNextDimensionToSplit(segment);
    ASSERT_EQ(0, dimensionWithSplitInfo.getSplitDimension());
    ASSERT_EQ(4.0, dimensionWithSplitInfo.getSplitInfo().getThreshold());
}