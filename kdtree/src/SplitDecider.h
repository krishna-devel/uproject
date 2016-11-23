#ifndef KDTREE_SPLITDECIDER_H
#define KDTREE_SPLITDECIDER_H

#include "Segment.h"

template <typename  DataType, typename DimensionType>
class SplitDecider {
public:
    static bool shouldSplit(const Segment<DataType, DimensionType> &segment);
};

template <typename  DataType, typename DimensionType>
bool SplitDecider<DataType, DimensionType>::shouldSplit(const Segment<DataType, DimensionType> &segment) {
    return segment.getSamples().rows() > 1;
}

#endif //KDTREE_SPLITDECIDER_H
