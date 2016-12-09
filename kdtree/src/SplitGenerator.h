#ifndef KDTREE_SPLITGENERATOR_H
#define KDTREE_SPLITGENERATOR_H

#include "Util.h"
#include "SegmentSplitter.h"
#include <string>

enum SplittingMethod {
    MEDIAN,
    MEDIAN_OF_MEDIAN
};

template <typename DataType, typename DimensionType>
class SplitGenerator {
public:
    static SplitWithSegments<DataType, DimensionType> generate(
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

        return SplitWithSegments<DataType, DimensionType>(
            Split<DataType, DimensionType>(dimensionToSplitBy, splitPoint, leftBounds, rightBounds),
            splitSegments
        );
    }

    static Point<DataType, DimensionType> getSplitPoint(
        const Segment<DataType, DimensionType> &segment,
        const SplittingMethod &splittingMethod,
        const DimensionType &dimensionToSplitBy
    ) {
        switch(splittingMethod) {
            case MEDIAN: return median(segment, dimensionToSplitBy);
            case MEDIAN_OF_MEDIAN: return medianOfMedianScorer(segment, dimensionToSplitBy);
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
