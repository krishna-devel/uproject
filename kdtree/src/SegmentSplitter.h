#ifndef KDTREE_SEGMENTSPLITTER_H
#define KDTREE_SEGMENTSPLITTER_H

#include "Segment.h"
#include "DimensionSelector.h"

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
private:
    Segment<DataType, DimensionType> segmentLessThanThreshold;
    Segment<DataType, DimensionType> segmentGreaterThanThreshold;
};

template <typename  DataType, typename DimensionType>
class SegmentSplitter {
public:
    static SplitSegments<DataType, DimensionType> split(
            const Segment<DataType, DimensionType> &segment,
            const DimensionWithSplitInfo<DataType, DimensionType> &dimensionWithSplitInfo
    );
};

template <typename  DataType, typename DimensionType>
SplitSegments<DataType, DimensionType> SegmentSplitter<DataType, DimensionType>::split(
        const Segment<DataType, DimensionType> &segment,
        const DimensionWithSplitInfo<DataType, DimensionType> &dimensionWithSplitInfo
) {
    const Samples<DataType> &samples = segment.getSamples();
    const SampleIdsInSegment<DimensionType> &sampleIdsInSegment = segment.getSampleIdsInSegment();

    DimensionType dimensionToSplit = dimensionWithSplitInfo.getSplitDimension();
    DataType splitThreshold = dimensionWithSplitInfo.getSplitInfo().getThreshold();

    SampleIdsInSegment<DimensionType> sampleIdsLessThanThreshold;
    SampleIdsInSegment<DimensionType> sampleIdsGreaterThanThreshold;

    for (DimensionType sampleId: sampleIdsInSegment) {
        DataType sampleValue = samples(sampleId, dimensionToSplit);
        if (sampleValue < splitThreshold) {
            sampleIdsLessThanThreshold.push_back(sampleId);
        } else if (sampleValue > splitThreshold) {
            sampleIdsGreaterThanThreshold.push_back(sampleId);
        }
    }

    Segment<DataType, DimensionType> segmentLessThanThreshold (samples, sampleIdsLessThanThreshold);
    Segment<DataType, DimensionType> segmentGreaterThanThreshold (samples, sampleIdsGreaterThanThreshold);
    SplitSegments<DataType, DimensionType> splitSegments (segmentLessThanThreshold, segmentGreaterThanThreshold);

    return splitSegments;
}

//class SimpleSegmentSplitter : public SegmentSplitter<DataType, DimensionType> {
//public:
//    virtual SplitSegments<DataType, DimensionType> split(
//        const Segment<DataType, DimensionType> &segment,
//        const DimensionWithSplitInfo<DataType> &dimensionWithSplitInfo
//    ) override {
//
//        const Samples<DataType> &samples = segment.getSamples();
//        const SampleIdsInSegment<DimensionType> &sampleIdsInSegment = segment.getSampleIdsInSegment();
//
//        DimensionType dimensionToSplit = dimensionWithSplitInfo.getSplitDimension();
//        DataType splitThreshold = dimensionWithSplitInfo.getSplitInfo().getThreshold();
//
//        SampleIdsInSegment<DimensionType> sampleIdsLessThanThreshold;
//        SampleIdsInSegment<DimensionType> sampleIdsGreaterThanThreshold;
//
//        for (DimensionType sampleId: sampleIdsInSegment) {
//            DataType sampleValue = samples(sampleId, dimensionToSplit);
//            if (sampleValue < splitThreshold) {
//                sampleIdsLessThanThreshold.push_back(sampleId);
//            } else {
//                sampleIdsGreaterThanThreshold.push_back(sampleId);
//            }
//        }
//
//        Segment<DataType, DimensionType> segmentLessThanThreshold (samples, sampleIdsLessThanThreshold);
//        Segment<DataType, DimensionType> segmentGreaterThanThreshold (samples, sampleIdsGreaterThanThreshold);
//        SplitSegments<DataType, DimensionType> splitSegments (segmentLessThanThreshold, segmentGreaterThanThreshold);
//
//        return splitSegments;
//    }
//};


#endif //KDTREE_SEGMENTSPLITTER_H
