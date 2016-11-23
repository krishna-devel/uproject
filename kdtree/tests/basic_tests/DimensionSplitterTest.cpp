#include "gtest/gtest.h"
#include <Dimension.h>
#include <DimensionSplitter.h>


TEST(DimensionSplitterTest, test_median_of_medians_with_no_values_in_dimension) {
    ASSERT_THROW(
            {
                 vector<ValueAlongDimension<double>> valuesAlongDimension;
                 SplitInfo<double> separationInfo = DimensionSplitter<double>::getSplitInfo(
                         DimensionSplittingMethod::MEDIAN_OF_MEDIAN,
                         valuesAlongDimension
                 );
             },
             cant_split_threshold_for_empty_vector_exception
    );
}

TEST(DimensionSplitterTest, test_median_of_medians_with_odd_number_of_points) {
    vector<ValueAlongDimension<double>> valuesAlongDimension {
        ValueAlongDimension<double>(0, 1.0),
        ValueAlongDimension<double>(1, 5.0),
        ValueAlongDimension<double>(2, 4.0),
        ValueAlongDimension<double>(3, 2.0),
        ValueAlongDimension<double>(4, 3.0)
    };

    SplitInfo<double> separationInfo = DimensionSplitter<double>::getSplitInfo(
            DimensionSplittingMethod::MEDIAN_OF_MEDIAN,
            valuesAlongDimension
    );

    ASSERT_EQ(3.0, separationInfo.getThreshold());
}

TEST(DimensionSplitterTest, test_median_of_medians_with_even_number_of_points) {
    vector<ValueAlongDimension<double>> valuesAlongDimension {
        ValueAlongDimension<double>(0, 1.0),
        ValueAlongDimension<double>(1, 5.0),
        ValueAlongDimension<double>(2, 4.0),
        ValueAlongDimension<double>(3, 2.0),
        ValueAlongDimension<double>(4, 3.0),
        ValueAlongDimension<double>(5, 6.0)
    };

    SplitInfo<double> separationInfo = DimensionSplitter<double>::getSplitInfo(
            DimensionSplittingMethod::MEDIAN_OF_MEDIAN,
            valuesAlongDimension
    );

    ASSERT_EQ(3.5, separationInfo.getThreshold());
}