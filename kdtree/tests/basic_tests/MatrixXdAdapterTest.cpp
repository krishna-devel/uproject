#include <gtest/gtest.h>
#include "MatrixXdAdapter.h"


class MatrixXdAdapterTest : public testing::Test {
protected:
    virtual void SetUp() {
        fMatrix << 1.f,2.f,3.f,4.f;
        dMatrix << 1.0,2.0,3.0,4.0;
    }

    Matrix<float, Dynamic, Dynamic> fMatrix {2, 2};
    Matrix<double, Dynamic, Dynamic> dMatrix {2, 2};
};

TEST_F(MatrixXdAdapterTest, check_rows) {
    ASSERT_EQ(2, fMatrix.rows());
    ASSERT_EQ(2, dMatrix.rows());
}