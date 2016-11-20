#ifndef KDTREE_MATRIXXDADAPTER_H
#define KDTREE_MATRIXXDADAPTER_H

#include "eigen3/Eigen/Dense"

using namespace std;
using namespace Eigen;
using Eigen::MatrixXd;
using Eigen::Matrix;

template <typename T>
class MatrixXdAdapter {

public:
    MatrixXdAdapter(const Matrix<T, Dynamic, Dynamic> &matrix);
//    MatrixXdAdapter(const double &matrix);

    int rows();

private:
//    double r;
    Matrix<T, Dynamic, Dynamic> matrix;
//    MatrixXd matrix;
};


//template <typename T>
//int MatrixXdAdapter<T>::rows() {
//    return 2;
//}
//
//template <typename T>
//MatrixXdAdapter<T>::MatrixXdAdapter(const double &r) : r(r) {}

template <typename T>
int MatrixXdAdapter<T>::rows() {
    return this->matrix.rows();
}

template <typename T>
MatrixXdAdapter<T>::MatrixXdAdapter(const Matrix<T, Dynamic, Dynamic> &matrix) : matrix(matrix) {}

#endif //KDTREE_MATRIXXDADAPTER_H
