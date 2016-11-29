#ifndef KDTREE_SPLITGENERATOR_H
#define KDTREE_SPLITGENERATOR_H

#include "Dimension.h"
#include "Util.h"
#include "SegmentSplitter.h"
#include <string>

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

enum SplittingMethod {
    MEDIAN1,
    MEDIAN_OF_MEDIAN1
};

template <typename DataType, typename DimensionType>
class SplitGenerator {
public:
    static Split<DataType, DimensionType> generate(
        const Segment<DataType, DimensionType> &segment,
        const SplittingMethod &splittingMethod,
        const DimensionType &dimensionToSplitBy
    ) {
        SegmentSplitter<DataType, DimensionType> segmentSplitter;

        Point<DataType, DimensionType> splitPoint = getSplitPoint(segment, splittingMethod, dimensionToSplitBy);

        SplitSegments<DataType, DimensionType> splitSegments =
            segmentSplitter.split(segment, splitPoint, dimensionToSplitBy);
        const Segment<DataType, DimensionType> &segmentLessThanThreshold = splitSegments.getSegmentLessThanThreshold();
        const Segment<DataType, DimensionType> &segmentGreaterThanThreshold =
                splitSegments.getSegmentGreaterThanThreshold();

        Bounds<DataType, DimensionType> leftBounds =
                Bounds<DataType, DimensionType>::generate(segmentLessThanThreshold);
        Bounds<DataType, DimensionType> rightBounds =
                Bounds<DataType, DimensionType>::generate(segmentGreaterThanThreshold);

        return Split<DataType, DimensionType>(dimensionToSplitBy, splitPoint, leftBounds, rightBounds);
    }

    static Point<DataType, DimensionType> getSplitPoint(
        const Segment<DataType, DimensionType> &segment,
        const SplittingMethod &splittingMethod,
        const DimensionType &dimensionToSplitBy
    ) {
        switch(splittingMethod) {
            case MEDIAN1: return median(segment, dimensionToSplitBy);
            case MEDIAN_OF_MEDIAN1: return medianOfMedianScorer(segment, dimensionToSplitBy);
        }
    }

private:
    static Point<DataType, DimensionType> median(
        const Segment<DataType, DimensionType> &segment,
        const DimensionType &dimensionToSplitBy
    );

    static Point<DataType, DimensionType> medianOfMedianScorer(
        const Segment<DataType, DimensionType> &segment,
        const DimensionType &dimensionToSplitBy
    );

    static vector<ValueInSegment<DataType, DimensionType>> getValuesInSegment(
        const Segment<DataType, DimensionType> &segment,
        const DimensionType &dimensionToSplitBy
    ) {
        vector<ValueInSegment<DataType, DimensionType>> valuesInSegment;
        for(DimensionType sampleId : segment.getSampleIdsInSegment()) {
            valuesInSegment.push_back(ValueInSegment<DataType, DimensionType>(segment, sampleId, dimensionToSplitBy));
        }
        return valuesInSegment;
    };
};


template <typename DataType, typename DimensionType>
Point<DataType, DimensionType> SplitGenerator<DataType, DimensionType>::median(
    const Segment<DataType, DimensionType> &segment,
    const DimensionType &dimensionToSplitBy
) {
    vector<ValueInSegment<DataType, DimensionType>> valuesInSegment = getValuesInSegment(segment, dimensionToSplitBy);
    DimensionType size = valuesInSegment.size();
    sort(valuesInSegment.begin(), valuesInSegment.end());
    if (size % 2 == 0) {
        Point<DataType, DimensionType> point1 = segment.getPoint(valuesInSegment[(size/2)-1].getSampleId());
        Point<DataType, DimensionType> point2 = segment.getPoint(valuesInSegment[size/2].getSampleId());
        return Point<DataType, DimensionType>::average(point1, point2);
    } else {
        return segment.getPoint(valuesInSegment[size/2].getSampleId());
    }
}

template <typename DataType, typename DimensionType>
Point<DataType, DimensionType> SplitGenerator<DataType, DimensionType>::medianOfMedianScorer(
    const Segment<DataType, DimensionType> &segment,
    const DimensionType &dimensionToSplitBy
) {
    vector<ValueInSegment<DataType, DimensionType>> valuesInSegment = getValuesInSegment(segment, dimensionToSplitBy);
    DimensionType size = valuesInSegment.size();
    DimensionType middleItemIndex = size / 2;
    nth_element(valuesInSegment.begin(), valuesInSegment.begin() + middleItemIndex, valuesInSegment.end());
    if (size % 2 == 0) {
        DimensionType neighborIndex = middleItemIndex - 1;
        nth_element(valuesInSegment.begin(), valuesInSegment.begin() + neighborIndex, valuesInSegment.end());
        Point<DataType, DimensionType> point1 = segment.getPoint(valuesInSegment[neighborIndex].getSampleId());
        Point<DataType, DimensionType> point2 = segment.getPoint(valuesInSegment[middleItemIndex].getSampleId());
        return Point<DataType, DimensionType>::average(point1, point2);
    } else {
        return segment.getPoint(valuesInSegment[middleItemIndex].getSampleId());
    }
}

#endif //KDTREE_SPLITGENERATOR_H