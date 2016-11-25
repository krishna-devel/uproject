#ifndef KDTREE_DIMENSION_H
#define KDTREE_DIMENSION_H

#include <vector>
#include "Segment.h"

using namespace std;

template <typename DataType, typename DimensionType>
class ValueAlongDimension {
public:
    ValueAlongDimension(DimensionType rowId, DataType value) : rowId(rowId), value(value) {}
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
    Dimension(const Segment<DataType, DimensionType> &segment, DimensionType dimensionId);
    const Segment<DataType, DimensionType> &getAdapter() const { return adapter; }
    const vector<ValueAlongDimension<DataType, DimensionType>> &getValuesAlongDimension() const {
        return valuesAlongDimension;
    }
private:
    const Segment<DataType, DimensionType> &adapter;
    DimensionType dimensionId;
    vector<ValueAlongDimension<DataType, DimensionType>> valuesAlongDimension;
};

template <typename DataType, typename DimensionType>
Dimension<DataType, DimensionType>::Dimension(
        const Segment<DataType, DimensionType> &segment,
        DimensionType dimensionId
) : adapter(segment), dimensionId(dimensionId) {
    const Samples<DataType> &samples = segment.getSamples();
    const SampleIdsInSegment<DimensionType> &indices = segment.getSampleIdsInSegment();
    if (dimensionId < samples.cols()) {
        for (DimensionType index: indices) {
            valuesAlongDimension.push_back(
                ValueAlongDimension<DataType, DimensionType>(index, samples(index, dimensionId))
            );
        }
    }
}

#endif //KDTREE_DIMENSION_H
