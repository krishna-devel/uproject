#include <gtest/gtest.h>
#include "Segment.h"

class SegmentTest : public testing::Test {
protected:
    virtual void SetUp() {
        fSamples <<
                1.f,2.f,
                3.f,4.f;
        dSamples <<
                1.0,2.0,
                3.0,4.0,
                5.0,6.0;
    }

    Samples<float> fSamples {2, 2};
    Samples<double> dSamples {3, 2};

};

TEST_F(SegmentTest, check_indices_are_correct_if_initialized_without_constructor) {
    Segment<float, int> segment (fSamples);
    vector<int> expectedIndices {0,1};

    ASSERT_EQ(expectedIndices, segment.getSampleIdsInSegment());
    ASSERT_EQ(fSamples.rows(), segment.getSampleIdsInSegment().size());
}

TEST_F(SegmentTest, check_indices_are_correct_if_initialized_with_constructor) {
    vector<long> passedIndices {0,2};
    Segment<double, long> segment (dSamples, passedIndices);

    vector<long> expectedIndices {0,2};
    ASSERT_EQ(expectedIndices, segment.getSampleIdsInSegment());
    ASSERT_EQ(fSamples.rows(), segment.getSampleIdsInSegment().size());
}

TEST_F(SegmentTest, test_ValueInSegment_works_as_expected) {
    Segment<float, int> segment (fSamples);
    ValueInSegment<float, int> valueInSegment (segment, 0, 1);
    ASSERT_EQ(2.0, valueInSegment.getValue());
}

TEST_F(SegmentTest, test_ValueInSegment_throws_exception_if_sample_id_is_not_in_segment) {
    ASSERT_THROW(
            ({
                Segment<float, int> segment (fSamples);
                ValueInSegment<float, int> valueInSegment (segment, 7, 0);
            }),
            sample_id_not_in_segment
    );
}

TEST_F(SegmentTest, test_ValueInSegment_sorts_correctly) {
    Segment<float, int> segment (fSamples);
    vector<ValueInSegment<float, int>> valuesInSegment;
    valuesInSegment.push_back(ValueInSegment<float, int>(segment, 1, 1));
    valuesInSegment.push_back(ValueInSegment<float, int>(segment, 0, 1));

    // Before sort
    ASSERT_EQ(4.0, valuesInSegment[0].getValue());
    ASSERT_EQ(2.0, valuesInSegment[1].getValue());

    sort(valuesInSegment.begin(), valuesInSegment.end());
    // After sort
    ASSERT_EQ(2.0, valuesInSegment[0].getValue());
    ASSERT_EQ(4.0, valuesInSegment[1].getValue());
}

TEST_F(SegmentTest, test_conversion_to_point) {
    Segment<float, int> segment (fSamples);
    vector<float> expectedVector1 {1.0, 2.0};
    vector<float> expectedVector2 {3.0, 4.0};
    ASSERT_EQ(expectedVector1, segment.getPoint(0).getCoefficients());
    ASSERT_EQ(expectedVector2, segment.getPoint(1).getCoefficients());
}

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