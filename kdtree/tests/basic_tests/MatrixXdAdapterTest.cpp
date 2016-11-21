#include <gtest/gtest.h>
#include "MatrixXdAdapter.h"


class MatrixXdAdapterTest : public testing::Test {
protected:
    virtual void SetUp() {
        fMatrix <<
                1.f,2.f,
                3.f,4.f;
        dMatrix <<
                1.0,2.0,
                3.0,4.0,
                5.0,6.0;
    }

    Matrix<float, Dynamic, Dynamic> fMatrix {2, 2};
    Matrix<double, Dynamic, Dynamic> dMatrix {3, 2};

};

TEST_F(MatrixXdAdapterTest, check_indices_are_correct_if_initialized_without_constructor) {
    MatrixXdAdapter<float> adapter (fMatrix);
    vector<int> expectedIndices {0,1};

    ASSERT_EQ(expectedIndices, adapter.getRowIndices());
    ASSERT_EQ(fMatrix.rows(), adapter.getRowIndices().size());
}

TEST_F(MatrixXdAdapterTest, check_indices_are_correct_if_initialized_with_constructor) {
    vector<int> passedIndices {0,2};
    MatrixXdAdapter<double> adapter (dMatrix, passedIndices);

    vector<int> expectedIndices {0,2};
    ASSERT_EQ(expectedIndices, adapter.getRowIndices());
    ASSERT_EQ(fMatrix.rows(), adapter.getRowIndices().size());
}
