#ifndef KDTREE_DIMENSION_H
#define KDTREE_DIMENSION_H

#include <vector>
#include "Segment.h"

using namespace std;

template <typename DataType, typename DimensionType>
class ValueAlongDimension {
public:
    ValueAlongDimension(int rowId, DataType value) : rowId(rowId), value(value) {}
    DimensionType getRowId() const { return rowId; }
    DataType getValue() const { return value; }
    bool operator<( const ValueAlongDimension& val ) const { return value < val.value; }
private:
    DimensionType rowId;
    DataType value;
};

template <typename DataType, typename DimensionType>
class Dimension {
public:
    Dimension(const Segment<DataType, DimensionType> &segment, int dimensionId);
    const Segment<DataType, DimensionType> &getAdapter() const { return adapter; }
    const vector<ValueAlongDimension<DataType, DimensionType>> &getValuesAlongDimension() const { return valuesAlongDimension; }
private:
    const Segment<DataType, DimensionType> &adapter;
    int dimensionId;
    vector<ValueAlongDimension<DataType, DimensionType>> valuesAlongDimension;
};

template <typename DataType, typename DimensionType>
Dimension<DataType, DimensionType>::Dimension(const Segment<DataType, DimensionType> &segment, int dimensionId) : adapter(segment), dimensionId(dimensionId) {
    const Matrix<DataType, Dynamic, Dynamic> &samples = segment.getSamples();
    const vector<int> &indices = segment.getSamplesInSegment();
    if (dimensionId < samples.cols()) {
        for (int index: indices) {
            valuesAlongDimension.push_back(ValueAlongDimension<DataType, DimensionType>(index, samples(index, dimensionId)));
        }
    }
}

#endif //KDTREE_DIMENSION_H
