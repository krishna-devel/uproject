#include "gtest/gtest.h"
#include <SplitGenerator.h>
#include <string>

class SplitGeneratorTest : public testing::Test {
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
    }
    vector<float> splitPointVector {1.0, 2.0, 3.0};
    vector<float> maxPointVector {4.0, 5.0, 6.0};
    vector<float> minPointVector {1.0, 2.0, 3.0};
    Point<float, int> *maxPoint;
    Point<float, int> *minPoint;
    Bounds<float, int> *bounds;
    Split<float, int> *split;

};

TEST_F(SplitGeneratorTest, test_bounds_serilization) {
    string boundsString = bounds->toString();
    Bounds<float, int> newBounds = Bounds<float, int>::fromString(boundsString);
    EXPECT_EQ(newBounds.getMaxPoint().getCoefficients(), maxPointVector);
    EXPECT_EQ(newBounds.getMinPoint().getCoefficients(), minPointVector);
}

TEST_F(SplitGeneratorTest, test_split_serilization) {
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