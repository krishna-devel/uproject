#ifndef KDTREE_DIMENSIONSELECTOR_H
#define KDTREE_DIMENSIONSELECTOR_H

#include "Node.h"
#include "Segment.h"
#include "Dimension.h"
#include "DimensionSplitter.h"

template <typename  DataType>
class DimensionWithSplitInfo {
public:
    DimensionWithSplitInfo(
        int splitDimension,
        const SplitInfo<DataType> &splitInfo
    ) : splitDimension(splitDimension), splitInfo(splitInfo) {}
    int getSplitDimension() const { return splitDimension; }
    const SplitInfo<DataType> &getSplitInfo() const { return splitInfo; }
private:
    int splitDimension;
    SplitInfo<DataType> splitInfo;
};

template <typename  DataType, typename DimensionType>
class DimensionSelector {
public:
    virtual DimensionWithSplitInfo<DataType> getNextDimensionToSplit(
            const Segment<DataType, DimensionType> &segment
    ) = 0;
};

template <typename  DataType, typename DimensionType>
 class LoopingDimensionSelector : public DimensionSelector<DataType, DimensionType> {
public:
    LoopingDimensionSelector(int lastSelectedDimension) : lastSelectedDimension(lastSelectedDimension) {}

    virtual DimensionWithSplitInfo<DataType> getNextDimensionToSplit(
            const Segment<DataType, DimensionType> &segment
    ) override {
        long numDimensions = segment.getSamples().cols();
        long nextDimension = (lastSelectedDimension + 1)%numDimensions;

        Dimension<DataType, DimensionType> dimension (segment, nextDimension);
        SplitInfo<DataType> splitInfo = DimensionSplitter<DataType, DimensionType>::getSplitInfo(
                DimensionSplittingMethod::MEDIAN_OF_MEDIAN,
                dimension.getValuesAlongDimension()
        );

        DimensionWithSplitInfo<DataType> dimensionWithSplitInfo (nextDimension, splitInfo);
        return dimensionWithSplitInfo;
    }

private:
    int lastSelectedDimension;
};


#endif //KDTREE_DIMENSIONSELECTOR_H
