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

TEST(PointTest, test_squaredDistance) {
    vector<float> v1 {15, 8};
    vector<float> v2 {4, 3};
    Point<float, int> point1(v1);
    Point<float, int> point2(v2);

    float distance1 = Point<float, int>::squaredDistance(point1, point2);
    ASSERT_EQ(146.0, distance1);
}

TEST(PointTest, test_euclideanDistance) {
    vector<float> v1 {15, 8};
    vector<float> v2 {4, 3};
    Point<float, int> point1(v1);
    Point<float, int> point2(v2);

    float distance1 = Point<float, int>::euclideanDistance(point1, point2);
    EXPECT_NEAR(sqrt(146.0), distance1, 0.01);
}

TEST(BoundsTest, test_distance_from_point) {
    vector<float> maxPoint {2, 2};
    vector<float> minPoint {1, 1};

    Bounds<float, int> bounds(maxPoint, minPoint);

    vector<float> point {1, 1};
    EXPECT_NEAR(0, bounds.distanceFromPoint(point), 0.001);

    vector<float> point2 {0, 0};
    EXPECT_NEAR(2, bounds.distanceFromPoint(point2), 0.001);

    vector<float> point3 {4, 4};
    EXPECT_NEAR(8, bounds.distanceFromPoint(point3), 0.001);
}

TEST(SplitTest, test_getDistancesFromSplits) {

    vector<float> splitPoint {0, 0};

    vector<float> minLeft {-4, 0};
    vector<float> maxLeft {-1, 4};

    vector<float> minRight {1, 0};
    vector<float> maxRight {4, 4};

    Bounds<float, int> leftBounds (maxLeft, minLeft);
    Bounds<float, int> rightBounds (maxRight, minRight);

    Split<float, int> split (0, splitPoint, leftBounds, rightBounds);

    vector<float> query1 = {-2, 2};
    pair<float, float> distances1 = split.getDistancesFromSplits(query1);
    EXPECT_NEAR(0, distances1.first, 0.001);
    EXPECT_NEAR(4, distances1.second, 0.001);

    vector<float> query2 = {2, 2};
    pair<float, float> distances2 = split.getDistancesFromSplits(query2);
    EXPECT_NEAR(4, distances2.first, 0.001);
    EXPECT_NEAR(0, distances2.second, 0.001);

    vector<float> query3 = {0, 0};
    pair<float, float> distances3 = split.getDistancesFromSplits(query3);
    EXPECT_NEAR(0, distances3.first, 0.001);
    EXPECT_NEAR(0, distances3.second, 0.001);

    vector<float> query4 = {0, -1};
    pair<float, float> distances4 = split.getDistancesFromSplits(query4);
    EXPECT_NEAR(1, distances4.first, 0.001);
    EXPECT_NEAR(1, distances4.second, 0.001);
}

class SegmentSplitterTest : public testing::Test {
protected:
    virtual void SetUp() {
        samplesWithOneRow << 1.0, 2.0, 3.0, 4.0, 5.0;
        samplesWithMultipleRows <<
                                1.0, 2.0, 3.0, 4.0, 5.0,
                6.0, 7.0, 8.0, 9.0, 10.0,
                11.0, 12.0, 13.0, 14.0, 15.0,
                16.0, 17.0, 18.0, 19.0, 20.0,
                21.0, 22.0, 23.0, 24.0, 25.0,
                26.0, 27.0, 28.0, 29.0, 30.0;
        samplesWithDuplicateRows <<
                                 1.0, 2.0, 3.0,
                1.0, 2.0, 3.0,
                1.0, 2.0, 3.0;

    }
    static bool isBalanced(const SplitSegments<float, int> &splitSegments) {
        long diff = labs(splitSegments.getSegmentLessThanThreshold().getSampleIdsInSegment().size() -
                         splitSegments.getSegmentGreaterThanThreshold().getSampleIdsInSegment().size()) <= 1;
        return diff;
    }
    Samples<float> samplesWithDuplicateRows {3, 3};
    Samples<float> samplesWithOneRow {1, 5};
    Samples<float> samplesWithMultipleRows {6, 5};
};

TEST_F(SegmentSplitterTest, split_base_case) {
    Segment<float, int> segment (samplesWithMultipleRows);

    vector<float> splitPointVector {11.0, 12.0, 13.0, 14.0, 15.0};
    Point<float, int> splitPoint(splitPointVector);

    SplitSegments<float, int> splitSegments = SplitSegments<float, int>::generate(segment, splitPoint, 2);

    SampleIdsInSegment<int> allSegmentIds {0,1,2,3,4,5};
    SampleIdsInSegment<int> expectedSegmentIdsInLessThan {0,1,2};
    SampleIdsInSegment<int> expectedSegmentIdsInGreaterThan {3,4,5};

    ASSERT_EQ(segment.getSamples(), splitSegments.getSegmentLessThanThreshold().getSamples());
    ASSERT_EQ(segment.getSamples(), splitSegments.getSegmentGreaterThanThreshold().getSamples());

    ASSERT_EQ(allSegmentIds, segment.getSampleIdsInSegment());
    ASSERT_EQ(expectedSegmentIdsInLessThan, splitSegments.getSegmentLessThanThreshold().getSampleIdsInSegment());
    ASSERT_EQ(expectedSegmentIdsInGreaterThan, splitSegments.getSegmentGreaterThanThreshold().getSampleIdsInSegment());
    EXPECT_TRUE(SegmentSplitterTest::isBalanced(splitSegments));
}

TEST_F(SegmentSplitterTest, split_sample_with_one_row) {
    Segment<float, int> segment (samplesWithOneRow);

    vector<float> splitPointVector {1.0, 2.0, 3.0, 4.0, 5.0};
    Point<float, int> splitPoint(splitPointVector);

    SplitSegments<float, int> splitSegments = SplitSegments<float, int>::generate(segment, splitPoint, 2);

    SampleIdsInSegment<int> allSegmentIds {0};
    SampleIdsInSegment<int> expectedSegmentIdsInLessThan {0};
    SampleIdsInSegment<int> expectedSegmentIdsInGreaterThan;

    ASSERT_EQ(segment.getSamples(), splitSegments.getSegmentLessThanThreshold().getSamples());
    ASSERT_EQ(segment.getSamples(), splitSegments.getSegmentGreaterThanThreshold().getSamples());

    ASSERT_EQ(allSegmentIds, segment.getSampleIdsInSegment());
    ASSERT_EQ(expectedSegmentIdsInLessThan, splitSegments.getSegmentLessThanThreshold().getSampleIdsInSegment());
    ASSERT_EQ(expectedSegmentIdsInGreaterThan, splitSegments.getSegmentGreaterThanThreshold().getSampleIdsInSegment());
    EXPECT_TRUE(SegmentSplitterTest::isBalanced(splitSegments));
}

TEST_F(SegmentSplitterTest, split_sample_with_duplicate_rows) {
    Segment<float, int> segment (samplesWithDuplicateRows);

    vector<float> splitPointVector {1.0, 2.0, 3.0};
    Point<float, int> splitPoint(splitPointVector);

    SplitSegments<float, int> splitSegments = SplitSegments<float, int>::generate(segment, splitPoint, 2);
    SampleIdsInSegment<int> allSegmentIds {0, 1, 2};
    SampleIdsInSegment<int> expectedSegmentIdsInLessThan {0, 2};
    SampleIdsInSegment<int> expectedSegmentIdsInGreaterThan {1};

    ASSERT_EQ(segment.getSamples(), splitSegments.getSegmentLessThanThreshold().getSamples());
    ASSERT_EQ(segment.getSamples(), splitSegments.getSegmentGreaterThanThreshold().getSamples());

    ASSERT_EQ(allSegmentIds, segment.getSampleIdsInSegment());
    ASSERT_EQ(expectedSegmentIdsInLessThan, splitSegments.getSegmentLessThanThreshold().getSampleIdsInSegment());
    ASSERT_EQ(expectedSegmentIdsInGreaterThan, splitSegments.getSegmentGreaterThanThreshold().getSampleIdsInSegment());
    EXPECT_TRUE(SegmentSplitterTest::isBalanced(splitSegments));
}
