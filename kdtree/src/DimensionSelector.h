#ifndef KDTREE_DIMENSIONSELECTOR_H
#define KDTREE_DIMENSIONSELECTOR_H

#include "Segment.h"
#include "Dimension.h"
#include "DimensionSplitter.h"
#include <string>
#include <sstream>

using namespace std;

template <typename  DataType, typename DimensionType>
class DimensionWithSplitInfo {
public:
    DimensionWithSplitInfo(
        DimensionType splitDimension,
        const SplitInfo<DataType> &splitInfo
    ) : splitDimension(splitDimension), splitInfo(splitInfo) {}
    DimensionType getSplitDimension() const { return splitDimension; }
    const SplitInfo<DataType> &getSplitInfo() const { return splitInfo; }
    string toString() { return to_string(splitDimension) + "," + splitInfo.toString(); }
    static DimensionWithSplitInfo<DataType, DimensionType> fromString(string objectStr) {
        vector<string> data;
        istringstream ss(objectStr);
        string token;
        while(getline(ss, token, ',')) { data.push_back(token); }
        DimensionType splitDimension = stol(data[0]);
        SplitInfo<DataType> splitInfo = SplitInfo<DataType>::fromString(data[1]);
        return DimensionWithSplitInfo<DataType, DimensionType>(splitDimension, splitInfo);
    };
private:
    DimensionType splitDimension;
    SplitInfo<DataType> splitInfo;
};

enum DimensionSelectorType {
    LOOPING
};

template <typename  DataType, typename DimensionType>
class DimensionSelector {
public:
    virtual DimensionWithSplitInfo<DataType, DimensionType> getNextDimensionToSplit(
            const Segment<DataType, DimensionType> &segment
    ) = 0;
    static DimensionSelector<DataType, DimensionType> *getByType(const DimensionSelectorType type);
};

template <typename  DataType, typename DimensionType>
class LoopingDimensionSelector : public DimensionSelector<DataType, DimensionType> {
public:
    LoopingDimensionSelector(DataType lastSelectedDimension) : lastSelectedDimension(lastSelectedDimension) {}
    virtual DimensionWithSplitInfo<DataType, DimensionType> getNextDimensionToSplit(
        const Segment<DataType, DimensionType> &segment
    ) override {
        DimensionType numDimensions = segment.getSamples().cols();
        DimensionType nextDimension = (lastSelectedDimension + 1)%numDimensions;
        Dimension<DataType, DimensionType> dimension (segment, nextDimension);
        SplitInfo<DataType> splitInfo = DimensionSplitter<DataType, DimensionType>::getSplitInfo(
            DimensionSplittingMethod::MEDIAN_OF_MEDIAN,
            dimension.getValuesAlongDimension()
        );
        DimensionWithSplitInfo<DataType, DimensionType> dimensionWithSplitInfo (nextDimension, splitInfo);
        return dimensionWithSplitInfo;
    }
private:
    DimensionType lastSelectedDimension;
};

#endif //KDTREE_DIMENSIONSELECTOR_H
