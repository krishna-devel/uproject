#ifndef KDTREE_SPLITGENERATOR_H
#define KDTREE_SPLITGENERATOR_H

#include "Dimension.h"
#include "Util.h"
#include "Point.h"
#include <string>

template <typename DataType, typename DimensionType>
class Bounds {
public:
    Bounds(
        const Point<DataType, DimensionType> &maxPoint,
        const Point<DataType, DimensionType> &minPoint
    ) : maxPoint(maxPoint), minPoint(minPoint) {}
    const Point<DataType, DimensionType> &getMaxPoint() const { return maxPoint; }
    const Point<DataType, DimensionType> &getMinPoint() const { return minPoint; }
    string toString() {
        map<string, string> m;
        m["maxPoint"] = maxPoint.toString();
        m["minPoint"] = minPoint.toString();
        return Util::convertMapToString(m, ":bounds:", ";bounds;");
    }
    static Bounds<DataType, DimensionType> fromString(string objectStr) {
        map<string, string> m = Util::convertStringToMap(objectStr, ":bounds:", ";bounds;");
        Point<DataType, DimensionType> maxPoint = Point<DataType, DimensionType>::fromString(m["maxPoint"]);
        Point<DataType, DimensionType> minPoint = Point<DataType, DimensionType>::fromString(m["minPoint"]);
        return Bounds<DataType, DimensionType>(maxPoint, minPoint);
    }
private:
    Point<DataType, DimensionType> maxPoint;
    Point<DataType, DimensionType> minPoint;
};

template <typename DataType, typename DimensionType>
class Split {
public:
    Split(
        DimensionType splitDimension,
        const Point<DataType, DimensionType> &splitPoint,
        const Bounds<DataType, DimensionType> &leftBounds,
        const Bounds<DataType, DimensionType> &rightBounds
    ) :
        splitDimension(splitDimension),
        splitPoint(splitPoint),
        leftBounds(leftBounds),
        rightBounds(rightBounds) {}

    DataType getSplitThreshold() const { return splitPoint.getCoefficients()[splitDimension]; }

    string toString() {
        map<string, string> m;
        m["splitDimension"] = to_string(splitDimension);
        m["splitPoint"] = splitPoint.toString();
        m["leftBounds"] = leftBounds.toString();
        m["rightBounds"] = rightBounds.toString();
        return Util::convertMapToString(m, ":split:", ";split;");
    }
    static Split<DataType, DimensionType> fromString(string objectStr) {
        map<string, string> m = Util::convertStringToMap(objectStr, ":split:", ";split;");
        DataType splitDimension = stol(m["splitDimension"]);
        Point<DataType, DimensionType> point = Point<DataType, DimensionType>::fromString(m["splitPoint"]);
        Bounds<DataType, DimensionType> leftBounds = Bounds<DataType, DimensionType>::fromString(m["leftBounds"]);
        Bounds<DataType, DimensionType> rightBounds = Bounds<DataType, DimensionType>::fromString(m["rightBounds"]);
        return Split<DataType, DimensionType>(splitDimension, point, leftBounds, rightBounds);
    }

    // getters
    DimensionType getSplitDimension() const { return splitDimension; }
    const Point<DataType, DimensionType> &getSplitPoint() const { return splitPoint; }
    const Bounds<DataType, DimensionType> &getLeftBounds() const { return leftBounds; }
    const Bounds<DataType, DimensionType> &getRightBounds() const { return rightBounds; }

private:
    DimensionType splitDimension;
    Point<DataType, DimensionType> splitPoint;
    Bounds<DataType, DimensionType> leftBounds;
    Bounds<DataType, DimensionType> rightBounds;
};

enum SplittingMethod {
    MEDIAN,
    MEDIAN_OF_MEDIAN
};

template <typename DataType, typename DimensionType>
class SplitGenerator {
public:
    /**
     * Not passing valuesAlongDimension as reference as this can be modified
     * by the underlying implementation.
     *
     * @param splittingMethod
     * @param valuesAlongDimension
     * @return
     */
    static Split<DataType, DimensionType> generate(
        const Segment<DataType, DimensionType> &segment,
        const SplittingMethod &splittingMethod,
        const DimensionType &dimensionToSplitBy
    ) {
        switch(splittingMethod) {
            case MEDIAN: return median(segment, dimensionToSplitBy);
            case MEDIAN_OF_MEDIAN: return medianOfMedianScorer(segment, dimensionToSplitBy);
        }
    };
private:
    static Split<DataType, DimensionType> median(
        const Segment<DataType, DimensionType> &segment,
        const DimensionType &dimensionToSplitBy
    );
    static Split<DataType, DimensionType> medianOfMedianScorer(
        const Segment<DataType, DimensionType> &segment,
        const DimensionType &dimensionToSplitBy
    );
};



#endif //KDTREE_SPLITGENERATOR_H
