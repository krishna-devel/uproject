#include "gtest/gtest.h"
#include <SplitPointSelector.h>
#include <string>

class SplitPointSelectorTest : public testing::Test {
protected:
    virtual void SetUp() {
        maxPoint = new Point<float, int>(maxPointVector);
        minPoint = new Point<float, int>(minPointVector);
        bounds = new Bounds<float, int>(*maxPoint, *minPoint);
        split = new Split<float, int>(
            1,
            splitPointVector,
            Bounds<float, int>(*maxPoint, *minPoint),
            Bounds<float, int>(*minPoint, *maxPoint)
        );

        samples5x5 <<
                1.0,2.0,3.0,4.0,5.0,
                6.0,7.0,8.0,9.0,10.0,
                11.0,12.0,13.0,14.0,15.0,
                16.0,17.0,18.0,19.0,20.0,
                21.0,22.0,23.0,24.0,25.0;

    }
    vector<float> splitPointVector {1.0, 2.0, 3.0};
    vector<float> maxPointVector {4.0, 5.0, 6.0};
    vector<float> minPointVector {1.0, 2.0, 3.0};
    Point<float, int> *maxPoint;
    Point<float, int> *minPoint;
    Bounds<float, int> *bounds;
    Split<float, int> *split;

    Samples<float> samples5x5 {5, 5};

};

TEST_F(SplitPointSelectorTest, test_bounds_serilization) {
    string boundsString = bounds->toString();
    Bounds<float, int> newBounds = Bounds<float, int>::fromString(boundsString);
    EXPECT_EQ(newBounds.getMaxPoint().getCoefficients(), maxPointVector);
    EXPECT_EQ(newBounds.getMinPoint().getCoefficients(), minPointVector);
}

TEST_F(SplitPointSelectorTest, test_split_serilization) {
    string splitString = split->toString();
    Split<float, int> newSplit = Split<float, int>::fromString(splitString);

    EXPECT_EQ(1, newSplit.getSplitDimension());
    EXPECT_EQ(2.0, newSplit.getSplitThreshold());
    EXPECT_EQ(newSplit.getSplitPoint().getCoefficients(), splitPointVector);
    EXPECT_EQ(newSplit.getLeftBounds().getMaxPoint().getCoefficients(), maxPointVector);
    EXPECT_EQ(newSplit.getLeftBounds().getMinPoint().getCoefficients(), minPointVector);
    EXPECT_EQ(newSplit.getRightBounds().getMaxPoint().getCoefficients(), minPointVector);
    EXPECT_EQ(newSplit.getRightBounds().getMinPoint().getCoefficients(), maxPointVector);
}

TEST_F(SplitPointSelectorTest, test_bounds_on_complete_segment) {
    Samples<double> samples {3, 2};
    samples <<
            1.0,2.0,
            3.0,4.0,
            5.0,6.0;
    Segment<double, int> segment(samples);
    Bounds<double, int> bounds = Bounds<double, int>::generate(segment);

    vector<double> max {5.0, 6.0};
    vector<double> min {1.0, 2.0};
    EXPECT_EQ(max, bounds.getMaxPoint().getCoefficients());
    EXPECT_EQ(min, bounds.getMinPoint().getCoefficients());
}

TEST_F(SplitPointSelectorTest, test_bounds_on_partial_segment) {
    Samples<double> samples {3, 2};
    vector<int> ids {1, 2};
    samples <<
            1.0,2.0,
            3.0,4.0,
            5.0,6.0;
    Segment<double, int> segment(samples, ids);
    Bounds<double, int> bounds = Bounds<double, int>::generate(segment);

    vector<double> max {5.0, 6.0};
    vector<double> min {3.0, 4.0};
    EXPECT_EQ(max, bounds.getMaxPoint().getCoefficients());
    EXPECT_EQ(min, bounds.getMinPoint().getCoefficients());
}

TEST_F(SplitPointSelectorTest, test_splitting_with_odd_metrics) {
    Segment<float, int> segment(samples5x5);

    Point<float, int> splitPoint1 = SplitPointSelector<float, int>::getSplitPoint(segment, SplittingMethod::MEDIAN, 2);
    ASSERT_EQ(13.0, splitPoint1.getCoefficients()[2]);

    Point<float, int> splitPoint2 = SplitPointSelector<float, int>::getSplitPoint(segment, SplittingMethod::MEDIAN_OF_MEDIAN, 2);
    ASSERT_EQ(13.0, splitPoint2.getCoefficients()[2]);
}

TEST_F(SplitPointSelectorTest, test_splitting_with_even_metrics) {
    vector<int> rows {0,1,3,4};
    Segment<float, int> segment(samples5x5, rows);

    Point<float, int> splitPoint1 = SplitPointSelector<float, int>::getSplitPoint(segment, SplittingMethod::MEDIAN, 1);
    ASSERT_EQ(12.0, splitPoint1.getCoefficients()[1]);

    Point<float, int> splitPoint2 = SplitPointSelector<float, int>::getSplitPoint(segment, SplittingMethod::MEDIAN_OF_MEDIAN, 1);
    ASSERT_EQ(12.0, splitPoint2.getCoefficients()[1]);
}

TEST_F(SplitPointSelectorTest, test_generate_entire_segment) {
    Segment<float, int> segment(samples5x5);

    SplitWithSegments<float, int> splitWithSegments =
            SplitPointSelector<float, int>::generate(segment, SplittingMethod::MEDIAN, 1);
    Split<float, int> split = splitWithSegments.getSplit();

    vector<float> expectedSplitPoint {11,12,13,14,15};
    vector<float> leftMin {1,2,3,4,5};
    vector<float> leftMax {11,12,13,14,15};
    vector<float> rightMin {16.0,17.0,18.0,19.0,20.0};
    vector<float> rightMax {21.0,22.0,23.0,24.0,25.0};

    EXPECT_EQ(1, split.getSplitDimension());
    EXPECT_EQ(expectedSplitPoint, split.getSplitPoint().getCoefficients());
    EXPECT_EQ(leftMin, split.getLeftBounds().getMinPoint().getCoefficients());
    EXPECT_EQ(leftMax, split.getLeftBounds().getMaxPoint().getCoefficients());
    EXPECT_EQ(rightMin, split.getRightBounds().getMinPoint().getCoefficients());
    EXPECT_EQ(rightMax, split.getRightBounds().getMaxPoint().getCoefficients());
}

TEST_F(SplitPointSelectorTest, test_generate_for_2_rows) {
    vector<int> rows {0,4};
    Segment<float, int> segment(samples5x5, rows);

    SplitWithSegments<float, int> splitWithSegments =
        SplitPointSelector<float, int>::generate(segment, SplittingMethod::MEDIAN, 1);
    Split<float, int> split = splitWithSegments.getSplit();

    vector<float> expectedSplitPoint {11,12,13,14,15};
    vector<float> leftMin {1,2,3,4,5};
    vector<float> leftMax {11,12,13,14,15};
    vector<float> rightMin {16.0,17.0,18.0,19.0,20.0};
    vector<float> rightMax {21.0,22.0,23.0,24.0,25.0};

    EXPECT_EQ(1, split.getSplitDimension());
    EXPECT_EQ(expectedSplitPoint, split.getSplitPoint().getCoefficients());
    EXPECT_EQ(leftMin, split.getLeftBounds().getMinPoint().getCoefficients());
    EXPECT_EQ(leftMin, split.getLeftBounds().getMaxPoint().getCoefficients());
    EXPECT_EQ(rightMax, split.getRightBounds().getMinPoint().getCoefficients());
    EXPECT_EQ(rightMax, split.getRightBounds().getMaxPoint().getCoefficients());
}
