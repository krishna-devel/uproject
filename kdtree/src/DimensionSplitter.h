#ifndef KDTREE_DIMENSIONSPLITTER_H
#define KDTREE_DIMENSIONSPLITTER_H

#include "Dimension.h"

using namespace std;

enum DimensionSplittingMethod {
    MEDIAN_OF_MEDIAN
};

template <typename  DataType>
class SplitInfo {
public:
    SplitInfo(DataType threshold) : splitThreshold(threshold) {}
    DataType getThreshold() const { return splitThreshold; }
private:
    DataType splitThreshold;
};

template <typename DataType, typename DimensionType>
class DimensionSplitter {
public:
    /**
     * Not passing valuesAlongDimension as reference as this can be modified
     * by the underlying implementation.
     *
     * @param splittingMethod
     * @param valuesAlongDimension
     * @return
     */
    static SplitInfo<DataType> getSplitInfo(
            const DimensionSplittingMethod splittingMethod,
            vector<ValueAlongDimension<DataType, DimensionType>> valuesAlongDimension
    );
private:
    static SplitInfo<DataType> medianOfMedianScorer(
            vector<ValueAlongDimension<DataType, DimensionType>> valuesAlongDimension
    );
};

class cant_split_threshold_for_empty_vector_exception : public exception {};

/**
 * http://stackoverflow.com/a/2579393
 *
 * @param valuesAlongDimension
 * @return
 */
template <typename DataType, typename DimensionType>
SplitInfo<DataType> DimensionSplitter<DataType, DimensionType>::medianOfMedianScorer(
        vector<ValueAlongDimension<DataType, DimensionType>> valuesAlongDimension
) {
    DataType threshold;

    DimensionType size = valuesAlongDimension.size();
    DimensionType middleItemIndex = size / 2;

    nth_element(
            valuesAlongDimension.begin(),
            valuesAlongDimension.begin() + middleItemIndex,
            valuesAlongDimension.end()
    );

    if (size % 2 != 0) {
        //Odd number of points
        threshold = valuesAlongDimension[middleItemIndex].getValue();
    } else {
        //Even number of points
        DimensionType neighborIndex = middleItemIndex - 1;
        nth_element(
                valuesAlongDimension.begin(),
                valuesAlongDimension.begin() + neighborIndex,
                valuesAlongDimension.end()
        );
        threshold = (valuesAlongDimension[neighborIndex].getValue() +
                        valuesAlongDimension[middleItemIndex].getValue()) / 2.0;
    }
    return SplitInfo<DataType>(threshold);
}

template <typename DataType, typename DimensionType>
SplitInfo<DataType> DimensionSplitter<DataType, DimensionType>::getSplitInfo(
        const DimensionSplittingMethod splittingMethod,
        vector<ValueAlongDimension<DataType, DimensionType>> valuesAlongDimension
) {
    DimensionType size = valuesAlongDimension.size();
    if (size != 0) {
        switch(splittingMethod) {
            case MEDIAN_OF_MEDIAN:
                return DimensionSplitter<DataType, DimensionType>::medianOfMedianScorer(valuesAlongDimension);
        }
    } else {
        throw cant_split_threshold_for_empty_vector_exception();
    }
}

#endif //KDTREE_DIMENSIONSPLITTER_H
