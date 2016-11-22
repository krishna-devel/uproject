#ifndef KDTREE_AXISSEPARATION_H
#define KDTREE_AXISSEPARATION_H

#include "Axis.h"
#include "Util.h"

enum AxisScoreType {
    MEDIAN
};

template <typename  T>
class SeparationInfo {
public:
    SeparationInfo(
        double separationScore,
        const ValueAlongAxis<T> &valueAlongAxis
    ): separationScore(separationScore), valueAlongAxis(valueAlongAxis) {}

    double getSeparationScore() const { return separationScore; }
    const ValueAlongAxis<T> &getValueAlongAxis() const { return valueAlongAxis; }

private:
    double separationScore;
    ValueAlongAxis<T> valueAlongAxis;
};

template <typename  T>
class AxisSeparation {

public:
    /**
     * Not passing valuesAlongAxis as reference as this can be modified
     * by the underlying implementation.
     *
     * @param scoreType
     * @param valuesAlongAxis
     * @return
     */
    static SeparationInfo<T> score(
            const AxisScoreType scoreType,
            const vector<ValueAlongAxis<T>> valuesAlongAxis
    );

private:
    static SeparationInfo<T> medianScorer(const vector<ValueAlongAxis<T>> valuesAlongAxis);

};

/**
 * http://stackoverflow.com/a/2579393
 *
 * @param valuesAlongAxis
 * @return
 */
template <typename  T>
SeparationInfo<T> AxisSeparation<T>::medianScorer(vector<ValueAlongAxis<T>> valuesAlongAxis) {

//    size_t middleItemIndex = valuesAlongAxis.size()/2;
//    nth_element(valuesAlongAxis.begin(), valuesAlongAxis.begin() + middleItemIndex, valuesAlongAxis.end());
//
//    if(size % 2 != 0){ //Odd number of elements
//        return *target;
//    }else{            //Even number of elements
//        double a = *target;
//        RandAccessIter targetNeighbor= target-1;
//        std::nth_element(begin, targetNeighbor, end);
//        return (a+*targetNeighbor)/2.0;
//    }

    return SeparationInfo<T>(1.0, valuesAlongAxis[0]);
}

template <typename  T>
SeparationInfo<T> AxisSeparation<T>::score(const AxisScoreType scoreType, const vector<ValueAlongAxis<T>> valuesAlongAxis) {
    switch(scoreType) {
        case MEDIAN: return AxisSeparation<T>::medianScorer(valuesAlongAxis);
    }
}

#endif //KDTREE_AXISSEPARATION_H
