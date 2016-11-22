#include "gtest/gtest.h"
#include <Axis.h>
#include <AxisSeparation.h>


class AxisSeparationTest : public testing::Test {
protected:
    virtual void SetUp() {}
};

TEST_F(AxisSeparationTest, test_median) {
    vector<ValueAlongAxis<double>> valuesAlongAxis {
        ValueAlongAxis<double>(0, 1.0),
        ValueAlongAxis<double>(1, 5.0),
        ValueAlongAxis<double>(2, 4.0),
        ValueAlongAxis<double>(3, 2.0),
        ValueAlongAxis<double>(4, 3.0)
    };

    sort(valuesAlongAxis.begin(), valuesAlongAxis.end());

    SeparationInfo<double> separationInfo = AxisSeparation<double>::score(AxisScoreType::MEDIAN, valuesAlongAxis);

    ASSERT_EQ(0, 0);
}