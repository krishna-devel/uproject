#ifndef KDTREE_SEGMENT_H
#define KDTREE_SEGMENT_H

#include <vector>
#include <numeric>
#include "eigen3/Eigen/Dense"

using namespace std;
using namespace Eigen;
using Eigen::Matrix;

template <typename DataType, typename DimensionType>
class Segment {
public:
    // constructors
    //TODO: Check samplesInSegment size is less than total samples and all items are valid.
    Segment(const Matrix<DataType, Dynamic, Dynamic> &samples) : samples(samples) {}
    Segment(
            const Matrix<DataType, Dynamic, Dynamic> &samples,
            const vector<DimensionType> &samplesInSegment
    ): samples(samples), samplesInSegment(samplesInSegment) {}
    // getters
    const Matrix<DataType, Dynamic, Dynamic> &getSamples() const { return samples; };
    const vector<DimensionType> &getSamplesInSegment() const { return samplesInSegment; };
private:
    Matrix<DataType, Dynamic, Dynamic> samples;
    vector<DimensionType> samplesInSegment;
};


//template <typename DataType, typename DimensionType>
//Segment<DataType, DimensionType>::Segment(const Matrix<DataType, Dynamic, Dynamic> &matrix) : samples(matrix) {
//    samplesInSegment = vector<DimensionType>(matrix.rows());
//    iota(begin(samplesInSegment), end(samplesInSegment), 0);
//}

//template <typename DataType, typename DimensionType>
//Segment<DataType, DimensionType>::Segment(
//        const Matrix<DataType, Dynamic, Dynamic> &matrix,
//        const vector<DimensionType> &indices
//) : samples(matrix), samplesInSegment(indices) {}
// Check samplesInSegment size is less than total samples and all items are valid.

#endif //KDTREE_SEGMENT_H
