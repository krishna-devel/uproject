#ifndef KDTREE_DIMENSIONSPLITTER_H
#define KDTREE_DIMENSIONSPLITTER_H

#include "Dimension.h"
#include <string>

using namespace std;

//template <typename DataType> using Point = vector<DataType>;

enum DimensionSplittingMethod {
    MEDIAN,
    MEDIAN_OF_MEDIAN
};

//template <typename  DataType>
//class Bounds {
//public:
//    Bounds(
//        const Point<DataType> &splittingPoint,
//        const Point<DataType> &minPoint,
//        const Point<DataType> &maxPoint
//    ) : splittingPoint(splittingPoint), minPoint(minPoint), maxPoint(maxPoint) {}
//    const Point<DataType> &getSplittingPoint() const { return splittingPoint; }
//    const Point<DataType> &getMinPoint() const { return minPoint; }
//    const Point<DataType> &getMaxPoint() const { return maxPoint; }
//private:
//    Point<DataType> splittingPoint;
//    Point<DataType> minPoint;
//    Point<DataType> maxPoint;
//};
template <typename  DataType>
class SplitInfo {
public:
    SplitInfo(DataType threshold) : splitThreshold(threshold) {}
    DataType getThreshold() const { return splitThreshold; }
    string toString() { return ":si:"+to_string(getThreshold())+":si:"; }
    static SplitInfo<DataType> fromString(string objectStr) {
        replace1(":si:", "", objectStr);
        DataType threshold = stod(objectStr);
        return SplitInfo(threshold);
    }
    static bool replace1(const string &from, const string &to, string &str) {
        size_t start_pos = str.find(from);
        if(start_pos == string::npos){
            return false;
        }
        str.replace(start_pos, from.length(), to);
        return true;
    }
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
    static SplitInfo<DataType> median(
        vector<ValueAlongDimension<DataType, DimensionType>> valuesAlongDimension
    );
    static SplitInfo<DataType> medianOfMedianScorer(
        vector<ValueAlongDimension<DataType, DimensionType>> valuesAlongDimension
    );
};

class cant_split_threshold_for_empty_vector_exception : public exception {};

template <typename DataType, typename DimensionType>
SplitInfo<DataType> DimensionSplitter<DataType, DimensionType>::median(
    vector<ValueAlongDimension<DataType, DimensionType>> valuesAlongDimension
) {
    DataType threshold;
    size_t size = valuesAlongDimension.size();
    sort(valuesAlongDimension.begin(), valuesAlongDimension.end());
    if (size  % 2 == 0) {
        threshold =  (valuesAlongDimension[(size/2)-1].getValue() + valuesAlongDimension[size/2].getValue()) / 2;
    } else {
        threshold = valuesAlongDimension[size/2].getValue();
    }
    return SplitInfo<DataType>(threshold);
}

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
    SplitInfo<DataType>* splitInfo;
    if (size != 0) {
        switch(splittingMethod) {
            case MEDIAN:
                return DimensionSplitter<DataType, DimensionType>::median(valuesAlongDimension);
            case MEDIAN_OF_MEDIAN:
                return DimensionSplitter<DataType, DimensionType>::medianOfMedianScorer(valuesAlongDimension);
        }
    } else {
        throw cant_split_threshold_for_empty_vector_exception();
    }
//    return *splitInfo;
}

#endif //KDTREE_DIMENSIONSPLITTER_H
