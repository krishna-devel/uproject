#ifndef KDTREE_AXIS_H
#define KDTREE_AXIS_H

#include <vector>
#include "EigenMatrixAdapter.h"

using namespace std;

template <typename T>
class ValueAlongAxis {
public:
    ValueAlongAxis(int rowId, T value) : rowId(rowId), value(value) {}

    int getRowId() const { return rowId; }
    T getValue() const { return value; }

    bool operator<( const ValueAlongAxis& val ) const {
        return value < val.value;
    }

private:
    int rowId;
    T value;
};

template <typename T>
class Axis {
public:
    // constructor
    Axis(const EigenMatrixAdapter<T> &adapter, int axisId);

    // getters
    const EigenMatrixAdapter<T> &getAdapter() const { return adapter; }
    int getAxisId() const { return axisId; }

    const vector<ValueAlongAxis<T>> &getValuesAlongAxis() const { return valuesAlongAxis; }

private:
    const EigenMatrixAdapter<T> &adapter;
    int axisId;
    vector<ValueAlongAxis<T>> valuesAlongAxis;

};

template <typename T>
Axis<T>::Axis(const EigenMatrixAdapter<T> &adapter, int axisId) : adapter(adapter), axisId(axisId) {

    const Matrix<T, Dynamic, Dynamic> &samples = adapter.getSamples();
    const vector<int> &indices = adapter.getRowIndices();

    if (axisId < samples.cols()) {
        for (int index: indices) {
            valuesAlongAxis.push_back(ValueAlongAxis<T>(index, samples(index, axisId)));
        }
    }
}

#endif //KDTREE_AXIS_H
