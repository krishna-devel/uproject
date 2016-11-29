#ifndef KDTREE_POINT_H
#define KDTREE_POINT_H

#include "vector"
#include "Util.h"
#include <numeric>

using namespace std;

template <typename DataType, typename DimensionType>
class Point {
public:
    Point(const vector<DataType> &coefficients) : coefficients(coefficients) {}
    const vector<DataType> &getCoefficients() const { return coefficients; }
    string toString() {
        vector<string> v;
        for (DataType c: coefficients) {
            v.push_back(to_string(c));
        }
        return Util::convertVectorToString(v, ";p;");
    }
    DataType getValueAt(DimensionType dimensionId) const {
        return coefficients[dimensionId];
    }
    static Point<DataType, DimensionType> fromString(string objectStr) {
        vector<DataType> coefficients;
        for (string s: Util::convertStringToVector(objectStr, ";p;")) {
            coefficients.push_back(stod(s));
        }
        return Point<DataType, DimensionType>(coefficients);
    }
    static Point<DataType, DimensionType> average(
        Point<DataType, DimensionType> point1,
        Point<DataType, DimensionType> point2
    ) {

        vector<DataType> point1Coefficients = point1.getCoefficients();
        vector<DataType> point2Coefficients = point2.getCoefficients();

        assert(point1Coefficients.size() == point2Coefficients.size());
        vector<DimensionType> x(point1.getCoefficients().size());
        std::iota(begin(x), end(x), 0);

        vector<DataType> averageCoefficients;
        for (DimensionType dimension : x) {
            DataType average = (point1Coefficients[dimension] + point2Coefficients[dimension])/2.0;
            averageCoefficients.push_back(average);
        }

        return Point<DataType, DimensionType>(averageCoefficients);
    };

private:
    vector<DataType> coefficients;
};

#endif //KDTREE_POINT_H
