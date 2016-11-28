#ifndef KDTREE_POINT_H
#define KDTREE_POINT_H

#include "vector"
#include "Util.h"

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
    static Point<DataType, DimensionType> fromString(string objectStr) {
        vector<DataType> coefficients;
        for (string s: Util::convertStringToVector(objectStr, ";p;")) {
            coefficients.push_back(stod(s));
        }
        return Point<DataType, DimensionType>(coefficients);
    }
private:
    vector<DataType> coefficients;
};

#endif //KDTREE_POINT_H
