#include <gtest/gtest.h>
#include "Axis.h"

class AxisTest : public testing::Test {
protected:
    virtual void SetUp() {
        dMatrix <<
                1.0, 2.0, 3.0,
                4.0, 5.0, 6.0,
                7.0, 8.0, 9.0;
    }

    Matrix<double, Dynamic, Dynamic> dMatrix {3, 3};

};

TEST_F(AxisTest, getValuesAlongAxis_checks_for_valid_index_size) {
    EigenMatrixAdapter<double> adapter1 (dMatrix);
    Axis<double> axis (adapter1, 5);
    ASSERT_EQ(0, axis.getValuesAlongAxis().size());
}

TEST_F(AxisTest, getValuesAlongAxis_returns_right_values) {
    EigenMatrixAdapter<double> adapter1 (dMatrix);
    Axis<double> axis1 (adapter1, 0);
    const vector<ValueAlongAxis<double>> valuesAlongAxis = axis1.getValuesAlongAxis();

    ASSERT_EQ(1.0, valuesAlongAxis[0].getValue());
    ASSERT_EQ(4.0, valuesAlongAxis[1].getValue());
    ASSERT_EQ(7.0, valuesAlongAxis[2].getValue());

    ASSERT_EQ(0, valuesAlongAxis[0].getRowId());
    ASSERT_EQ(1, valuesAlongAxis[1].getRowId());
    ASSERT_EQ(2, valuesAlongAxis[2].getRowId());
}

TEST_F(AxisTest, getValuesAlongAxis_returns_right_values_for_partial_row_indices) {
    vector<int> indices {0, 2};
    EigenMatrixAdapter<double> adapter2 (dMatrix, indices);
    Axis<double> axis2 (adapter2, 1);

    const vector<ValueAlongAxis<double>> valuesAlongAxis = axis2.getValuesAlongAxis();
    ASSERT_EQ(2.0, valuesAlongAxis[0].getValue());
    ASSERT_EQ(8.0, valuesAlongAxis[1].getValue());

    ASSERT_EQ(0, valuesAlongAxis[0].getRowId());
    ASSERT_EQ(2, valuesAlongAxis[1].getRowId());
}
