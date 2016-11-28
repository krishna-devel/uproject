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

class sample_id_not_in_segment : public exception {};

template <typename DataType, typename DimensionType>
class ValueInSegment {
public:
    ValueInSegment(
        const Segment<DataType, DimensionType> &segment,
        DimensionType sampleId,
        DimensionType dimensionId
    ) : segment(segment), sampleId(sampleId), dimensionId(dimensionId) {
        bool sampleIdIsPresent = false;
        for(DimensionType d: segment.getSampleIdsInSegment()) {
            if (d == sampleId) {
                sampleIdIsPresent = true;
                break;
            }
        }
        if (!sampleIdIsPresent) {
            throw sample_id_not_in_segment();
        }
    }
    DataType getValue() const { return segment.getSamples()(sampleId, dimensionId); }
    bool operator<( const ValueInSegment& val ) const { return getValue() < val.getValue(); }
private:
    Segment<DataType, DimensionType> segment;
    DimensionType sampleId;
    DimensionType dimensionId;
};

#endif //KDTREE_SEGMENT_H
