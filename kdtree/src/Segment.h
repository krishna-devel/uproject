#ifndef KDTREE_SEGMENT_H
#define KDTREE_SEGMENT_H

#include "eigen3/Eigen/Dense"
#include "Util.h"
#include <memory>
#include <numeric>
#include <vector>
#include <math.h>

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

    static DataType squaredDistance(Point<DataType, DimensionType> point1, Point<DataType, DimensionType> point2) {

        vector<DataType> point1Coefficients = point1.getCoefficients();
        vector<DataType> point2Coefficients = point2.getCoefficients();

        vector<DimensionType> x(point1.getCoefficients().size());
        std::iota(begin(x), end(x), 0);

        DataType distance = 0;

        for (DimensionType dimension : x) {
            DataType diff = (point1Coefficients[dimension] - point2Coefficients[dimension]);
            distance += (diff*diff);
        }
        if (distance <= 0) return 0; else return distance;
    };

    static DataType euclideanDistance(Point<DataType, DimensionType> point1, Point<DataType, DimensionType> point2) {
        DataType squaredDistance = Point<DataType, DimensionType>::squaredDistance(point1, point2);
        return sqrt(squaredDistance);
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
    const Samples<DataType> samples;
    SampleIdsInSegment<DimensionType> sampleIdsInSegment;
};

template <typename DataType, typename DimensionType>
class ValueInSegment {
public:
    ValueInSegment(
        const Segment<DataType, DimensionType> &segment,
        DimensionType sampleId,
        DimensionType dimensionId
    ) : sampleId(sampleId), dimensionId(dimensionId) {
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
        value = segment.getSamples()(sampleId, dimensionId);
    }
    DataType getValue() const { return value; }
    bool operator<( const ValueInSegment& val ) const { return getValue() < val.getValue(); }

    DimensionType getSampleId() const { return sampleId; }

private:
    DataType value;
    DimensionType sampleId;
    DimensionType dimensionId;
};

template <typename  DataType, typename DimensionType>
class SplitSegments {
public:
    SplitSegments(
        const Segment<DataType, DimensionType> &segmentLessThanThreshold,
        const Segment<DataType, DimensionType> &segmentgreaterThanThreshold
    ) : segmentLessThanThreshold(segmentLessThanThreshold), segmentGreaterThanThreshold(segmentgreaterThanThreshold) {}

    const Segment<DataType, DimensionType> &getSegmentLessThanThreshold() const { return segmentLessThanThreshold; }

    const Segment<DataType, DimensionType> &getSegmentGreaterThanThreshold() const {
        return segmentGreaterThanThreshold;
    }

    static SplitSegments<DataType, DimensionType> generate(
            const Segment<DataType, DimensionType> &segment,
            const Point<DataType, DimensionType> &splitPoint,
            const DimensionType &dimensionToSplitBy) {
        const Samples<DataType> &samples = segment.getSamples();
        const SampleIdsInSegment<DimensionType> &sampleIdsInSegment = segment.getSampleIdsInSegment();

        DataType splitThreshold = splitPoint.getValueAt(dimensionToSplitBy);

        SampleIdsInSegment<DimensionType> sampleIdsLessThanThreshold;
        SampleIdsInSegment<DimensionType> sampleIdsGreaterThanThreshold;

        bool putInLessThanIfEqual = true;
        for (DimensionType sampleId: sampleIdsInSegment) {
            DataType sampleValue = samples(sampleId, dimensionToSplitBy);
            if (sampleValue < splitThreshold) {
                sampleIdsLessThanThreshold.push_back(sampleId);
            } else if (sampleValue > splitThreshold) {
                sampleIdsGreaterThanThreshold.push_back(sampleId);
            } else {
                if (putInLessThanIfEqual) {
                    sampleIdsLessThanThreshold.push_back(sampleId);
                    putInLessThanIfEqual = false;
                } else {
                    sampleIdsGreaterThanThreshold.push_back(sampleId);
                    putInLessThanIfEqual = true;
                }
            }
        }

        Segment<DataType, DimensionType> segmentLessThanThreshold (samples, sampleIdsLessThanThreshold);
        Segment<DataType, DimensionType> segmentGreaterThanThreshold (samples, sampleIdsGreaterThanThreshold);
        SplitSegments<DataType, DimensionType> splitSegments (segmentLessThanThreshold, segmentGreaterThanThreshold);

        return splitSegments;
    }


private:
    const Segment<DataType, DimensionType> segmentLessThanThreshold;
    const Segment<DataType, DimensionType> segmentGreaterThanThreshold;
};

template <typename DataType, typename DimensionType>
class Bounds {
public:
    Bounds(
        const Point<DataType, DimensionType> &maxPoint,
        const Point<DataType, DimensionType> &minPoint
    ) : maxPoint(maxPoint), minPoint(minPoint) {}
    const Point<DataType, DimensionType> &getMaxPoint() const { return maxPoint; }
    const Point<DataType, DimensionType> &getMinPoint() const { return minPoint; }
    string toString() {
        map<string, string> m;
        m["maxPoint"] = maxPoint.toString();
        m["minPoint"] = minPoint.toString();
        return Util::convertMapToString(m, ":bounds:", ";bounds;");
    }
    static Bounds<DataType, DimensionType> fromString(string objectStr) {
        map<string, string> m = Util::convertStringToMap(objectStr, ":bounds:", ";bounds;");
        Point<DataType, DimensionType> maxPoint = Point<DataType, DimensionType>::fromString(m["maxPoint"]);
        Point<DataType, DimensionType> minPoint = Point<DataType, DimensionType>::fromString(m["minPoint"]);
        return Bounds<DataType, DimensionType>(maxPoint, minPoint);
    }
    static Bounds<DataType, DimensionType> generate(const Segment<DataType, DimensionType> &segment) {
        DimensionType numDimensions = segment.getNumDimensions();
        const Samples<DataType> &samples = segment.getSamples();
        const SampleIdsInSegment<DimensionType> &sampleIdsInSegment = segment.getSampleIdsInSegment();

        vector<DataType> maxValues (numDimensions);
        vector<DataType> minValues (numDimensions);

        for (DimensionType d = 0; d < numDimensions; d++) {
            maxValues[d] = samples(sampleIdsInSegment[0], d);
            minValues[d] = samples(sampleIdsInSegment[0], d);
        }

        for (DimensionType r = 1; r < sampleIdsInSegment.size(); r++) {
            for (DimensionType d = 0; d < numDimensions; d++) {
                if (maxValues[d] < samples(sampleIdsInSegment[r],d)) {
                    maxValues[d] = samples(sampleIdsInSegment[r],d);
                }
                if (minValues[d] > samples(sampleIdsInSegment[r],d)) {
                    minValues[d] = samples(sampleIdsInSegment[r],d);
                }
            }
        }

        return Bounds<DataType, DimensionType>(
                Point<DataType, DimensionType>(maxValues),
                Point<DataType, DimensionType>(minValues)
        );
    };

    DataType distanceFromPoint(const Point<DataType, DimensionType> &point) const {
        vector<DataType> pointCoefficients = point.getCoefficients();
        vector<DataType> maxCoefficients = this->maxPoint.getCoefficients();
        vector<DataType> minCoefficients = this->minPoint.getCoefficients();
        vector<DimensionType> indices(pointCoefficients.size());
        iota(begin(indices), end(indices), 0);

        DataType distance = 0.0;
        for (DimensionType index: indices) {
            DataType coefficient = pointCoefficients[index];
            DataType maxCoefficient = maxCoefficients[index];
            DataType minCoefficient = minCoefficients[index];
            if (minCoefficient > coefficient) {
                DataType diff = (minCoefficient - coefficient);
                distance += (diff * diff);
            } else if (maxCoefficient < coefficient) {
                DataType diff = (coefficient - maxCoefficient);
                distance += (diff * diff);
            }
        }

        return distance;
    }
private:
    Point<DataType, DimensionType> maxPoint;
    Point<DataType, DimensionType> minPoint;
};

template <typename DataType, typename DimensionType>
class Split {
public:
    Split(
            DimensionType splitDimension,
            const Point<DataType, DimensionType> &splitPoint,
            const Bounds<DataType, DimensionType> &leftBounds,
            const Bounds<DataType, DimensionType> &rightBounds
    ) :
            splitDimension(splitDimension),
            splitPoint(splitPoint),
            leftBounds(leftBounds),
            rightBounds(rightBounds) {}

    DataType getSplitThreshold() const { return splitPoint.getCoefficients()[splitDimension]; }

    const pair<DataType, DataType> getDistancesFromSplits(const Point<DataType, DimensionType> &query) {

        vector<DataType> maxLeft = leftBounds.getMaxPoint().getCoefficients();
        vector<DataType> minLeft = leftBounds.getMinPoint().getCoefficients();
        maxLeft[splitDimension] = splitPoint.getCoefficients()[splitDimension];

        vector<DataType> maxRight = rightBounds.getMaxPoint().getCoefficients();
        vector<DataType> minRight = rightBounds.getMinPoint().getCoefficients();
        minRight[splitDimension] = splitPoint.getCoefficients()[splitDimension];

        return make_pair(
                Bounds<DataType, DimensionType>(maxLeft, minLeft).distanceFromPoint(query),
                Bounds<DataType, DimensionType>(maxRight, minRight).distanceFromPoint(query)
        );
    }

    string toString() {
        map<string, string> m;
        m["splitDimension"] = to_string(splitDimension);
        m["splitPoint"] = splitPoint.toString();
        m["leftBounds"] = leftBounds.toString();
        m["rightBounds"] = rightBounds.toString();
        return Util::convertMapToString(m, ":split:", ";split;");
    }
    static Split<DataType, DimensionType> fromString(string objectStr) {
        map<string, string> m = Util::convertStringToMap(objectStr, ":split:", ";split;");
        DataType splitDimension = stol(m["splitDimension"]);
        Point<DataType, DimensionType> point = Point<DataType, DimensionType>::fromString(m["splitPoint"]);
        Bounds<DataType, DimensionType> leftBounds = Bounds<DataType, DimensionType>::fromString(m["leftBounds"]);
        Bounds<DataType, DimensionType> rightBounds = Bounds<DataType, DimensionType>::fromString(m["rightBounds"]);
        return Split<DataType, DimensionType>(splitDimension, point, leftBounds, rightBounds);
    }

    // getters
    DimensionType getSplitDimension() const { return splitDimension; }
    const Point<DataType, DimensionType> &getSplitPoint() const { return splitPoint; }
    const Bounds<DataType, DimensionType> &getLeftBounds() const { return leftBounds; }
    const Bounds<DataType, DimensionType> &getRightBounds() const { return rightBounds; }

private:
    DimensionType splitDimension;
    Point<DataType, DimensionType> splitPoint;
    Bounds<DataType, DimensionType> leftBounds;
    Bounds<DataType, DimensionType> rightBounds;
};

template <typename DataType, typename DimensionType>
class SplitWithSegments {
public:
    SplitWithSegments(
            const Split<DataType, DimensionType> &split,
            const SplitSegments<DataType, DimensionType> &splitSegments
    ) : split(split), splitSegments(splitSegments) {}
    const Split<DataType, DimensionType> &getSplit() const { return split; }
    const SplitSegments<DataType, DimensionType> &getSplitSegments() const { return splitSegments; }
private:
    Split<DataType, DimensionType> split;
    SplitSegments<DataType, DimensionType> splitSegments;
};


#endif //KDTREE_SEGMENT_H
