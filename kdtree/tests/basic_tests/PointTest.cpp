#include <gmock/gmock.h>
#include "Point.h"

TEST(PointTest, test_serialization) {
    vector<float> v {1.0, 2.0, 3.0};
    Point<float, int> point(v);
    string pointString = point.toString();
    Point<float, int> newPoint = Point<float, int>::fromString(pointString);
    EXPECT_EQ(point.getCoefficients(), newPoint.getCoefficients());
}