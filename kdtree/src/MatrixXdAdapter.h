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
    const vector<int> &getRowIndices() const { return rowIndices; };

private:
    Matrix<T, Dynamic, Dynamic> samples;
    vector<int> rowIndices;
};

template <typename T>
MatrixXdAdapter<T>::MatrixXdAdapter(const Matrix<T, Dynamic, Dynamic> &matrix) : samples(matrix) {
    rowIndices = vector<int>(matrix.rows());
    iota(begin(rowIndices), end(rowIndices), 0);
}

template <typename T>
MatrixXdAdapter<T>::MatrixXdAdapter(
        const Matrix<T, Dynamic, Dynamic> &matrix,
        const vector<int> &indices
) : samples(matrix), rowIndices(indices) {}
// Check rowIndices size is less than rows and all items are valid.

#endif //KDTREE_MATRIXXDADAPTER_H
