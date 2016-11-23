#ifndef KDTREE_SEGMENT_H
#define KDTREE_SEGMENT_H

#include <vector>
#include <numeric>
#include "eigen3/Eigen/Dense"

using namespace std;
using namespace Eigen;
using Eigen::Matrix;

template <typename DataType> using Samples = Matrix<DataType, Dynamic, Dynamic>;
template <typename DimensionType> using SampleIdsInSegment = vector<DimensionType>;

template <typename DataType, typename DimensionType>
class Segment {
public:
    //TODO: Check sampleIdsInSegment size is less than total samples and all items are valid.
    Segment(const Samples<DataType> &samples) : samples(samples) {
        sampleIdsInSegment = vector<DimensionType>(samples.rows());
        iota(begin(sampleIdsInSegment), end(sampleIdsInSegment), 0);
    }
    Segment(
            const Samples<DataType> &samples,
            const vector<DimensionType> &samplesInSegment
    ): samples(samples), sampleIdsInSegment(samplesInSegment) {}
    const Samples<DataType> &getSamples() const { return samples; };
    const SampleIdsInSegment<DimensionType> &getSampleIdsInSegment() const { return sampleIdsInSegment; };
private:
    Samples<DataType> samples;
    SampleIdsInSegment<DimensionType> sampleIdsInSegment;
};

#endif //KDTREE_SEGMENT_H
