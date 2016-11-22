#include "gtest/gtest.h"
#include <Axis.h>
#include <AxisSeparation.h>


TEST(AxisSeparationTest, test_median_empty_Axis) {
    ASSERT_THROW(
            {
                 vector<ValueAlongAxis<double>> valuesAlongAxis;
                 SeparationInfo<double> separationInfo = AxisSeparation<double>::score(
                         AxisScoreType::MEDIAN_OF_MEDIAN,
                         valuesAlongAxis
                 );
             },
             cant_split_threshold_for_empty_vector_exception
    );
}

TEST(AxisSeparationTest, test_median_odd_numbers) {
    vector<ValueAlongAxis<double>> valuesAlongAxis {
        ValueAlongAxis<double>(0, 1.0),
        ValueAlongAxis<double>(1, 5.0),
        ValueAlongAxis<double>(2, 4.0),
        ValueAlongAxis<double>(3, 2.0),
        ValueAlongAxis<double>(4, 3.0)
    };

    SeparationInfo<double> separationInfo = AxisSeparation<double>::score(
            AxisScoreType::MEDIAN_OF_MEDIAN,
            valuesAlongAxis
    );

    ASSERT_EQ(3.0, separationInfo.getThreshold());
}

TEST(AxisSeparationTest, test_median_even_numbers) {
    vector<ValueAlongAxis<double>> valuesAlongAxis {
        ValueAlongAxis<double>(0, 1.0),
        ValueAlongAxis<double>(1, 5.0),
        ValueAlongAxis<double>(2, 4.0),
        ValueAlongAxis<double>(3, 2.0),
        ValueAlongAxis<double>(4, 3.0),
        ValueAlongAxis<double>(5, 6.0)
    };

    SeparationInfo<double> separationInfo = AxisSeparation<double>::score(
            AxisScoreType::MEDIAN_OF_MEDIAN,
            valuesAlongAxis
    );

    ASSERT_EQ(3.5, separationInfo.getThreshold());
}