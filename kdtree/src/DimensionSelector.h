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

template <typename  DataType, typename DimensionType>
class DimensionSelector {
public:
    virtual DimensionType getNextDimension(const Segment<DataType, DimensionType> &segment) = 0;
    DimensionWithSplitInfo<DataType, DimensionType> getNextDimensionToSplit(
        const Segment<DataType, DimensionType> &segment,
        const DimensionSplittingMethod dimensionSplittingMethod
    ) {
        const DimensionType nextDimension = getNextDimension(segment);
        Dimension<DataType, DimensionType> dimension (segment, nextDimension);
        SplitInfo<DataType> splitInfo = DimensionSplitter<DataType, DimensionType>::getSplitInfo(
//                DimensionSplittingMethod::MEDIAN_OF_MEDIAN,
                dimensionSplittingMethod,
                dimension.getValuesAlongDimension()
        );
        DimensionWithSplitInfo<DataType, DimensionType> dimensionWithSplitInfo (nextDimension, splitInfo);
        return dimensionWithSplitInfo;
    }
};

template <typename  DataType, typename DimensionType>
class CycleThroughAxesDimensionSelector : public DimensionSelector<DataType, DimensionType> {
public:
    CycleThroughAxesDimensionSelector(DataType lastSelectedDimension) : lastSelectedDimension(lastSelectedDimension) {}
    virtual DimensionType getNextDimension(const Segment<DataType, DimensionType> &segment) override {
        DimensionType numDimensions = segment.getSamples().cols();
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
        DimensionType numDimensions = segment.getSamples().cols();
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
