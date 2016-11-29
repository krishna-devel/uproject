#include <gmock/gmock.h>
#include "Point.h"

TEST(PointTest, test_serialization) {
    vector<float> v {1.0, 2.0, 3.0};
    Point<float, int> point(v);
    string pointString = point.toString();
    Point<float, int> newPoint = Point<float, int>::fromString(pointString);
    EXPECT_EQ(point.getCoefficients(), newPoint.getCoefficients());
}

TEST(PointTest, test_average) {
    vector<float> v1 {1.0, 2.0, 3.0};
    vector<float> v2 {3.0, 4.0, 7.0};

    Point<float, int> point1(v1);
    Point<float, int> point2(v2);

    vector<float> expectedVector {2.0, 3.0, 5.0};
    Point<float, int> point3 = Point<float, int>::average(point1, point2);

    ASSERT_EQ(expectedVector, point3.getCoefficients());
}

TEST(PointTest, test_get_value_at_dimension) {
    vector<float> v {1.0, 2.0, 3.0};
    Point<float, int> point(v);

    ASSERT_EQ(1.0, point.getValueAt(0));
    ASSERT_EQ(2.0, point.getValueAt(1));
    ASSERT_EQ(3.0, point.getValueAt(2));
}