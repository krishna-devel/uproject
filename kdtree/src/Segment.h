#ifndef KDTREE_SEGMENT_H
#define KDTREE_SEGMENT_H

#include <vector>
#include <numeric>
#include "eigen3/Eigen/Dense"

using namespace std;
using namespace Eigen;
using Eigen::Matrix;

template <typename T>
class Segment {
public:
    // constructors
    Segment() {};
    Segment(const Matrix<T, Dynamic, Dynamic> &matrix);
    Segment(const Matrix<T, Dynamic, Dynamic> &matrix, const vector<int> &indices);
    // getters
    const Matrix<T, Dynamic, Dynamic> &getSamples() const { return samples; };
    const vector<int> &getSamplesInSegment() const { return samplesInSegment; };
private:
    Matrix<T, Dynamic, Dynamic> samples;
    vector<int> samplesInSegment;
};


template <typename T>
Segment<T>::Segment(const Matrix<T, Dynamic, Dynamic> &matrix) : samples(matrix) {
    samplesInSegment = vector<int>(matrix.rows());
    iota(begin(samplesInSegment), end(samplesInSegment), 0);
}

template <typename T>
Segment<T>::Segment(
        const Matrix<T, Dynamic, Dynamic> &matrix,
        const vector<int> &indices
) : samples(matrix), samplesInSegment(indices) {}
// Check samplesInSegment size is less than total samples and all items are valid.

#endif //KDTREE_SEGMENT_H
