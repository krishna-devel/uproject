#include <gtest/gtest.h>
#include "Dimension.h"

class DimensionTest : public testing::Test {
protected:
    virtual void SetUp() {
        dMatrix <<
                1.0, 2.0, 3.0,
                4.0, 5.0, 6.0,
                7.0, 8.0, 9.0;
    }

    Matrix<double, Dynamic, Dynamic> dMatrix {3, 3};

};

TEST_F(DimensionTest, getValuesAlongDimension_checks_for_valid_index_size) {
    Segment<double> adapter1 (dMatrix);
    Dimension<double> dimension (adapter1, 5);
    ASSERT_EQ(0, dimension.getValuesAlongDimension().size());
}

TEST_F(DimensionTest, getValuesAlongDimension_returns_right_values) {
    Segment<double> adapter1 (dMatrix);
    Dimension<double> dimension1 (adapter1, 0);
    const vector<ValueAlongDimension<double>> valuesAlongDimension = dimension1.getValuesAlongDimension();

    ASSERT_EQ(1.0, valuesAlongDimension[0].getValue());
    ASSERT_EQ(4.0, valuesAlongDimension[1].getValue());
    ASSERT_EQ(7.0, valuesAlongDimension[2].getValue());

    ASSERT_EQ(0, valuesAlongDimension[0].getRowId());
    ASSERT_EQ(1, valuesAlongDimension[1].getRowId());
    ASSERT_EQ(2, valuesAlongDimension[2].getRowId());
}

TEST_F(DimensionTest, getValuesAlongDimension_returns_right_values_for_partial_row_indices) {
    vector<int> indices {0, 2};
    Segment<double> adapter2 (dMatrix, indices);
    Dimension<double> dimension2 (adapter2, 1);

    const vector<ValueAlongDimension<double>> valuesAlongDimension = dimension2.getValuesAlongDimension();
    ASSERT_EQ(2.0, valuesAlongDimension[0].getValue());
    ASSERT_EQ(8.0, valuesAlongDimension[1].getValue());

    ASSERT_EQ(0, valuesAlongDimension[0].getRowId());
    ASSERT_EQ(2, valuesAlongDimension[1].getRowId());
}
