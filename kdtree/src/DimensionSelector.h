#ifndef KDTREE_DIMENSIONSELECTOR_H
#define KDTREE_DIMENSIONSELECTOR_H

#include "Segment.h"
#include "Dimension.h"
#include "DimensionSplitter.h"
#include <string>
#include <sstream>
#include "Util.h"

using namespace std;

//template <typename  DataType, typename DimensionType>
//class DimensionWithSplitInfo {
//public:
//    DimensionWithSplitInfo(
//        DimensionType splitDimension,
//        const SplitInfo<DataType> &splitInfo
//    ) : splitDimension(splitDimension), splitInfo(splitInfo) {}
//    DimensionType getSplitDimension() const { return splitDimension; }
//    const SplitInfo<DataType> &getSplitInfo() const { return splitInfo; }
//    string toString() {
//        map<string, string> m;
//        m["splitDimension"] = to_string(splitDimension);
//        m["splitInfo"] = splitInfo.toString();
//        return Util::convertMapToString(m, ":ds:", ";ds;");
//    }
//    static DimensionWithSplitInfo<DataType, DimensionType> fromString(string objectStr) {
//        map<string, string> m = Util::convertStringToMap(objectStr, ":ds:", ";ds;");
//        DimensionType splitDimension = stol(m["splitDimension"]);
//        SplitInfo<DataType> splitInfo = SplitInfo<DataType>::fromString(m["splitInfo"]);
//        return DimensionWithSplitInfo<DataType, DimensionType>(splitDimension, splitInfo);
//    };
//private:
//    DimensionType splitDimension;
//    SplitInfo<DataType> splitInfo;
//};

template <typename  DataType, typename DimensionType>
class DimensionSelector {
public:
    virtual DimensionType getNextDimension(const Segment<DataType, DimensionType> &segment) = 0;
//    DimensionWithSplitInfo<DataType, DimensionType> getNextDimensionToSplit(
//        const Segment<DataType, DimensionType> &segment,
//        const DimensionSplittingMethod dimensionSplittingMethod
//    ) {
//        return getNextDimension(segment);
////        Dimension<DataType, DimensionType> dimension (segment, nextDimension);
////        SplitInfo<DataType> splitInfo = DimensionSplitter<DataType, DimensionType>::getSplitInfo(
////            dimensionSplittingMethod,
////            dimension.getValuesAlongDimension()
////        );
////        DimensionWithSplitInfo<DataType, DimensionType> dimensionWithSplitInfo (nextDimension, splitInfo);
////        return dimensionWithSplitInfo;
//    }

//    DimensionWithSplitInfo<DataType, DimensionType> getNextDimensionToSplit(
//        const Segment<DataType, DimensionType> &segment,
//        const DimensionSplittingMethod dimensionSplittingMethod
//    ) {
//        const DimensionType nextDimension = getNextDimension(segment);
//        Dimension<DataType, DimensionType> dimension (segment, nextDimension);
//        SplitInfo<DataType> splitInfo = DimensionSplitter<DataType, DimensionType>::getSplitInfo(
//            dimensionSplittingMethod,
//            dimension.getValuesAlongDimension()
//        );
//        DimensionWithSplitInfo<DataType, DimensionType> dimensionWithSplitInfo (nextDimension, splitInfo);
//        return dimensionWithSplitInfo;
//    }
};

template <typename  DataType, typename DimensionType>
class CycleThroughAxesDimensionSelector : public DimensionSelector<DataType, DimensionType> {
public:
    CycleThroughAxesDimensionSelector(DataType lastSelectedDimension) : lastSelectedDimension(lastSelectedDimension) {}
    virtual DimensionType getNextDimension(const Segment<DataType, DimensionType> &segment) override {
        DimensionType numDimensions = segment.getNumDimensions();
        return (lastSelectedDimension + 1)%numDimensions;
    }
private:
    DimensionType lastSelectedDimension;
};

template <typename  DataType, typename DimensionType>
class HighestRangeAxisDimensionSelector : public DimensionSelector<DataType, DimensionType> {
public:
    virtual DimensionType getNextDimension(const Segment<DataType, DimensionType> &segment) override {
        return getDimensionWithHighestRange(segment);
    }
private:
    DimensionType getDimensionWithHighestRange(const Segment<DataType, DimensionType> &segment) {
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

        DimensionType maxIndex = 0;
        DataType maxValue = maxValues[maxIndex] - minValues[maxIndex];
        for (DimensionType d = 0; d < numDimensions; d++) {
            DataType currentDiff = maxValues[d] - minValues[d];
            if (maxValue < currentDiff) {
                maxValue = currentDiff;
                maxIndex = d;
            }
        }

        return maxIndex;

    }
};

enum DimensionSelectorType {
    CYCLE_THROUGH_AXES,
    HIGHEST_RANGE_AXIS
};

template <typename  DataType, typename DimensionType>
class DimensionSelectorByType {
public:
    static DimensionSelector<DataType, DimensionType> *get(
        const DimensionSelectorType dimensionSelectorType,
        const DataType lastDimensionUsedForSplitting
    ) {
        switch(dimensionSelectorType) {
            case DimensionSelectorType::CYCLE_THROUGH_AXES:
                return new CycleThroughAxesDimensionSelector<DataType, DimensionType>(lastDimensionUsedForSplitting);
            case DimensionSelectorType::HIGHEST_RANGE_AXIS:
                return new HighestRangeAxisDimensionSelector<DataType, DimensionType>();
        }
    }
};

#endif //KDTREE_DIMENSIONSELECTOR_H
