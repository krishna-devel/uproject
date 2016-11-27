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
    CycleThroughAxesDimensionSelector<float, int> dimensionSelector (-1);
    DimensionWithSplitInfo<float, int> dimensionWithSplitInfo = dimensionSelector.getNextDimensionToSplit(segment);
    ASSERT_EQ(0, dimensionWithSplitInfo.getSplitDimension());
    ASSERT_EQ(4.0, dimensionWithSplitInfo.getSplitInfo().getThreshold());
}

TEST_F(LoopingDimensionSelectorTest, getNextDimensionToSplit_dimension_in_between) {
    Segment<float, int> segment (samples);
    CycleThroughAxesDimensionSelector<float, int> dimensionSelector (1);
    DimensionWithSplitInfo<float, int> dimensionWithSplitInfo = dimensionSelector.getNextDimensionToSplit(segment);
    ASSERT_EQ(2, dimensionWithSplitInfo.getSplitDimension());
    ASSERT_EQ(6.0, dimensionWithSplitInfo.getSplitInfo().getThreshold());
}

TEST_F(LoopingDimensionSelectorTest, getNextDimensionToSplit_dimension_rotate_back_to_first_dimension) {
    Segment<float, int> segment (samples);
    CycleThroughAxesDimensionSelector<float, int> dimensionSelector (2);
    DimensionWithSplitInfo<float, int> dimensionWithSplitInfo = dimensionSelector.getNextDimensionToSplit(segment);
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

TEST_F(LoopingDimensionSelectorTest, test_string_serilization) {
    samples.colwise().maxCoeff();
}