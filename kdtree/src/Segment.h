#ifndef KDTREE_SEGMENT_H
#define KDTREE_SEGMENT_H

#include "eigen3/Eigen/Dense"
#include "Util.h"
#include <memory>
#include <numeric>
#include <vector>

using namespace std;
using namespace Eigen;
using Eigen::Matrix;

template <typename DataType> using Samples = Matrix<DataType, Dynamic, Dynamic>;
template <typename DimensionType> using SampleIdsInSegment = vector<DimensionType>;

class sample_id_not_in_segment : public exception {};

template <typename DataType, typename DimensionType>
class Point {
public:
    Point(const vector<DataType> &coefficients) : coefficients(coefficients) {}
    const vector<DataType> &getCoefficients() const { return coefficients; }
    string toString() {
        vector<string> v;
        for (DataType c: coefficients) {
            v.push_back(to_string(c));
        }
        return Util::convertVectorToString(v, ";p;");
    }
    DataType getValueAt(DimensionType dimensionId) const {
        return coefficients[dimensionId];
    }
    static Point<DataType, DimensionType> fromString(string objectStr) {
        vector<DataType> coefficients;
        for (string s: Util::convertStringToVector(objectStr, ";p;")) {
            coefficients.push_back(stod(s));
        }
        return Point<DataType, DimensionType>(coefficients);
    }
    static Point<DataType, DimensionType> average(
            Point<DataType, DimensionType> point1,
            Point<DataType, DimensionType> point2
    ) {

        vector<DataType> point1Coefficients = point1.getCoefficients();
        vector<DataType> point2Coefficients = point2.getCoefficients();

        assert(point1Coefficients.size() == point2Coefficients.size());
        vector<DimensionType> x(point1.getCoefficients().size());
        std::iota(begin(x), end(x), 0);

        vector<DataType> averageCoefficients;
        for (DimensionType dimension : x) {
            DataType average = (point1Coefficients[dimension] + point2Coefficients[dimension])/2.0;
            averageCoefficients.push_back(average);
        }

        return Point<DataType, DimensionType>(averageCoefficients);
    };

private:
    vector<DataType> coefficients;
};

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
    Point<DataType, DimensionType> getPoint(DimensionType sampleId) const {
        vector<DimensionType> x(getNumDimensions());
        iota(begin(x), end(x), 0);
        vector<DataType> coefficients;
        for (DimensionType dimension : x) {
            coefficients.push_back(samples(sampleId, dimension));
        }
        return Point<DataType, DimensionType>(coefficients);
    };
    DimensionType getNumDimensions() const  { return samples.cols(); }
private:
    Samples<DataType> samples;
    SampleIdsInSegment<DimensionType> sampleIdsInSegment;
};

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

    DimensionType getSampleId() const { return sampleId; }

private:
    Segment<DataType, DimensionType> segment;
    DimensionType sampleId;
    DimensionType dimensionId;
};

#endif //KDTREE_SEGMENT_H
