#ifndef KDTREE_DIMENSIONSELECTOR_H
#define KDTREE_DIMENSIONSELECTOR_H

#include "Node.h"
#include "Segment.h"
#include "Dimension.h"

template <typename  T>
class DimensionWithSplitInfo {
public:
    DimensionWithSplitInfo(
        int splitDimension,
        const SplitInfo<T> &splitInfo
    ) : splitDimension(splitDimension), splitInfo(splitInfo) {}
    int getSplitDimension() const { return splitDimension; }
    const SplitInfo<T> &getSplitInfo() const { return splitInfo; }
private:
    int splitDimension;
    SplitInfo<T> splitInfo;
};

template <typename  T>
class DimensionSelector {
public:
    virtual DimensionWithSplitInfo getNextDimensionToSplit(const Segment<T> &segment) = 0;
};

//template <typename T>
// class LoopingDimensionSelector : public DimensionSelector<T> {
//public:
//    LoopingDimensionSelector(int lastSelectedDimension) : lastSelectedDimension(lastSelectedDimension) {}
//
//    virtual DimensionWithSplitInfo getNextDimensionToSplit(const Segment<T> &segment) override {
//        long numDimensions = segment.getSamples().cols();
//        long nextDimension = (lastSelectedDimension + 1)%numDimensions;
//
//        Dimension<T> dimension (segment, nextDimension);
//    }
//
//private:
//    int lastSelectedDimension;
//};


#endif //KDTREE_DIMENSIONSELECTOR_H
