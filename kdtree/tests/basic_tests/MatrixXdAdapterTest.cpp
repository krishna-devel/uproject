//
// Created by Krishna Kamath on 11/19/16.
//


#include <gtest/gtest.h>
#include "MatrixXdAdapter.h"

TEST(MatrixXdAdapter_test, check_rows) {
    MatrixXd m(2,2);
    m(0,0) = 3;
    m(1,0) = 2.5;
    m(0,1) = -1;
    m(1,1) = m(1,0) + m(0,1);

    MatrixXdAdapter matrixXdAdapter (m);

    ASSERT_EQ(2, matrixXdAdapter.rows());
}