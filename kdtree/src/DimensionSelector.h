#ifndef KDTREE_DIMENSIONSELECTOR_H
#define KDTREE_DIMENSIONSELECTOR_H

#include "Node.h"
#include "Segment.h"

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

class DimensionSelector {
public:
    virtual DimensionWithSplitInfo getNextDimensionToSplit(const Segment &segment) = 0;
};

class LoopingDimensionSelector : public DimensionSelector {

};


#endif //KDTREE_DIMENSIONSELECTOR_H
