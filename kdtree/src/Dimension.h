#ifndef KDTREE_DIMENSION_H
#define KDTREE_DIMENSION_H

#include <vector>
#include "EigenMatrixAdapter.h"

using namespace std;

template <typename T>
class ValueAlongDimension {
public:
    ValueAlongDimension(int rowId, T value) : rowId(rowId), value(value) {}
    int getRowId() const { return rowId; }
    T getValue() const { return value; }
    bool operator<( const ValueAlongDimension& val ) const { return value < val.value; }
private:
    int rowId;
    T value;
};

template <typename T>
class Dimension {
public:
    Dimension(const EigenMatrixAdapter<T> &adapter, int dimensionId);
    const EigenMatrixAdapter<T> &getAdapter() const { return adapter; }
    const vector<ValueAlongDimension<T>> &getValuesAlongDimension() const { return valuesAlongDimension; }
private:
    const EigenMatrixAdapter<T> &adapter;
    int dimensionId;
    vector<ValueAlongDimension<T>> valuesAlongDimension;
};

template <typename T>
Dimension<T>::Dimension(const EigenMatrixAdapter<T> &adapter, int dimensionId) : adapter(adapter), dimensionId(dimensionId) {

    const Matrix<T, Dynamic, Dynamic> &samples = adapter.getSamples();
    const vector<int> &indices = adapter.getRowIndices();

    if (dimensionId < samples.cols()) {
        for (int index: indices) {
            valuesAlongDimension.push_back(ValueAlongDimension<T>(index, samples(index, dimensionId)));
        }
    }
}

#endif //KDTREE_DIMENSION_H
