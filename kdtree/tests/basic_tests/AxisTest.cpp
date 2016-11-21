#include <gtest/gtest.h>
#include "Axis.h"
#include "MatrixXdAdapter.h"


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
    MatrixXdAdapter<double> adapter1 (dMatrix);
    Axis<double> axis (adapter1, 5);
    ASSERT_EQ(0, axis.getValuesAlongAxis().size());
}


TEST_F(AxisTest, getValuesAlongAxis_returns_right_values) {
    MatrixXdAdapter<double> adapter1 (dMatrix);
    Axis<double> axis1 (adapter1, 0);
    vector<double> expectedvalues1 {1.0, 4.0, 7.0};
    ASSERT_EQ(expectedvalues1, axis1.getValuesAlongAxis());

    vector<int> indices {0, 2};
    MatrixXdAdapter<double> adapter2 (dMatrix, indices);
    Axis<double> axis2 (adapter2, 1);
    vector<double> expectedvalues2 {2.0, 8.0};
    ASSERT_EQ(expectedvalues2, axis2.getValuesAlongAxis());
}
