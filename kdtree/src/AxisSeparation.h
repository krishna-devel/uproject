#ifndef KDTREE_AXISSEPARATION_H
#define KDTREE_AXISSEPARATION_H

#include "Axis.h"
#include "Util.h"

using namespace std;

enum AxisScoreType {
    MEDIAN_OF_MEDIAN
};

template <typename  T>
class SeparationInfo {
public:
    SeparationInfo(double score, T threshold) : score(score), threshold(threshold) {}
    double getScore() const { return score; }
    T getThreshold() const { return threshold; }
private:
    double score;
    T threshold;
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
            vector<ValueAlongAxis<T>> valuesAlongAxis
    );

//    static const string EMPTY_VECTOR_EXCEPTION_MESSAGE;

private:
    static SeparationInfo<T> medianOfMedianScorer(vector<ValueAlongAxis<T>> valuesAlongAxis);

};

class cant_split_threshold_for_empty_vector_exception : public exception {};

//template <typename  T>
//const string AxisSeparation<T>::EMPTY_VECTOR_EXCEPTION_MESSAGE = "Can't find split threshold for empty vector.";

/**
 * http://stackoverflow.com/a/2579393
 *
 * @param valuesAlongAxis
 * @return
 */
template <typename  T>
SeparationInfo<T> AxisSeparation<T>::medianOfMedianScorer(vector<ValueAlongAxis<T>> valuesAlongAxis) {
    T threshold;

    int size = valuesAlongAxis.size();
    int middleItemIndex = size / 2;

    nth_element(valuesAlongAxis.begin(), valuesAlongAxis.begin() + middleItemIndex, valuesAlongAxis.end());

    if (size % 2 != 0) {
        //Odd number of points
        threshold = valuesAlongAxis[middleItemIndex].getValue();
    } else {
        //Even number of points
        int neighborIndex = middleItemIndex - 1;
        nth_element(valuesAlongAxis.begin(), valuesAlongAxis.begin() + neighborIndex, valuesAlongAxis.end());
        threshold = (valuesAlongAxis[neighborIndex].getValue() + valuesAlongAxis[middleItemIndex].getValue()) / 2.0;
    }
    return SeparationInfo<T>(1.0, threshold);
}

template <typename  T>
SeparationInfo<T> AxisSeparation<T>::score(const AxisScoreType scoreType, vector<ValueAlongAxis<T>> valuesAlongAxis) {
    int size = valuesAlongAxis.size();
    if (size != 0) {
        switch(scoreType) {
            case MEDIAN_OF_MEDIAN: return AxisSeparation<T>::medianOfMedianScorer(valuesAlongAxis);
        }
    } else {
        throw cant_split_threshold_for_empty_vector_exception();
    }
}

#endif //KDTREE_AXISSEPARATION_H
