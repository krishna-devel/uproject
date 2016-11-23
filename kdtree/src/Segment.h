#ifndef KDTREE_SEGMENT_H
#define KDTREE_SEGMENT_H

#include <vector>
#include <numeric>
#include "eigen3/Eigen/Dense"

using namespace std;
using namespace Eigen;
using Eigen::Matrix;

template <typename DataType> using Samples = Matrix<DataType, Dynamic, Dynamic>;

template <typename DataType, typename DimensionType>
class Segment {
public:
    //TODO: Check samplesInSegment size is less than total samples and all items are valid.
    Segment(const Samples<DataType> &samples) : samples(samples) {
        samplesInSegment = vector<DimensionType>(samples.rows());
        iota(begin(samplesInSegment), end(samplesInSegment), 0);
    }
    Segment(
            const Samples<DataType> &samples,
            const vector<DimensionType> &samplesInSegment
    ): samples(samples), samplesInSegment(samplesInSegment) {}
    const Samples<DataType> &getSamples() const { return samples; };
    const vector<DimensionType> &getSamplesInSegment() const { return samplesInSegment; };
private:
    Samples<DataType> samples;
    vector<DimensionType> samplesInSegment;
};

#endif //KDTREE_SEGMENT_H
