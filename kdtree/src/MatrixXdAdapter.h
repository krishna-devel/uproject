#ifndef KDTREE_MATRIXXDADAPTER_H
#define KDTREE_MATRIXXDADAPTER_H

#include <vector>
#include <numeric>
#include "eigen3/Eigen/Dense"

using namespace std;
using namespace Eigen;
using Eigen::Matrix;

template <typename T>
class MatrixXdAdapter {

public:
    // constructors
    MatrixXdAdapter(const Matrix<T, Dynamic, Dynamic> &matrix);
    MatrixXdAdapter(const Matrix<T, Dynamic, Dynamic> &matrix, const vector<int> &indices);

    // getters
    const Matrix<T, Dynamic, Dynamic> &getSamples() const { return samples; };
    const vector<int> &getIndices() const { return indices; };

    //
//    const vector<T> &getValuesAlongAxis(int axis);

private:
    Matrix<T, Dynamic, Dynamic> samples;
    vector<int> indices;
};

template <typename T>
MatrixXdAdapter<T>::MatrixXdAdapter(const Matrix<T, Dynamic, Dynamic> &matrix) : samples(matrix) {
    this->indices = vector<int>(matrix.rows());
    iota(begin(this->indices), end(this->indices), 0);
}

template <typename T>
MatrixXdAdapter<T>::MatrixXdAdapter(
        const Matrix<T, Dynamic, Dynamic> &matrix,
        const vector<int> &indices
) : samples(matrix), indices(indices) {}

//template <typename T>
//const vector<T>& MatrixXdAdapter<T>::getValuesAlongAxis(int axis) {
//    return samples.col(axis);
//}


#endif //KDTREE_MATRIXXDADAPTER_H
