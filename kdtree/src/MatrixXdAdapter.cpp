#include "MatrixXdAdapter.h"

int MatrixXdAdapter::rows() {
    return this->matrix.rows();
}

MatrixXdAdapter::MatrixXdAdapter(const MatrixXd &matrix) : matrix(matrix) {}

