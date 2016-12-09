#ifndef KDTREE_DIMENSIONSELECTOR_H
#define KDTREE_DIMENSIONSELECTOR_H

#include "Segment.h"
#include <string>
#include <sstream>
#include "Util.h"

using namespace std;

enum DimensionSelectorType {
    CYCLE_THROUGH_AXES,
    HIGHEST_RANGE_AXIS
};

template <typename  DataType, typename DimensionType>
class DimensionSelectorByType {
public:
    static DimensionType get(
        const Segment<DataType, DimensionType> &segment,
        const DimensionSelectorType dimensionSelectorType,
        const DataType lastDimensionUsedForSplitting
    ) {
        switch(dimensionSelectorType) {
            case DimensionSelectorType::CYCLE_THROUGH_AXES:
                return cycleThroughAxis(segment, lastDimensionUsedForSplitting);
            case DimensionSelectorType::HIGHEST_RANGE_AXIS:
                return highestRangeAxis(segment);
        }
    }

private:
    static DimensionType cycleThroughAxis(
        const Segment<DataType, DimensionType> &segment,
        const DimensionType lastDimensionUsedForSplitting
    ) {
        DimensionType numDimensions = segment.getNumDimensions();
        return (lastDimensionUsedForSplitting + 1) % numDimensions;
    }

    static DimensionType highestRangeAxis(const Segment<DataType, DimensionType> &segment) {
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

#endif //KDTREE_DIMENSIONSELECTOR_H
