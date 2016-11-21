#ifndef KDTREE_AXISSCORER_H
#define KDTREE_AXISSCORER_H

#include "Axis.h"
#include "Util.h"

enum AxisScore {
    RANDOM
};

template <typename  T>
class AxisScorer {

public:
    void score(const Axis<T> &axis, double *separationScore, T *separationValue);

private:
    void randomScorer(const Axis<T> &axis, double *separationScore, T *separationValue);

};

template <typename  T>
void AxisScorer<T>::randomScorer(const Axis<T> &axis, double *separationScore, T *separationValue) {
    MatrixXdAdapter<T> adapter = axis.getAdapter();
    const vector<int> rowIndices = adapter.getRowIndices();
    const Matrix<T, Dynamic, Dynamic> samples = adapter.getSamples();

    int rowIndex = rowIndices[Util::getRandomInteger(rowIndices.size() - 1)];
    *separationValue = samples(rowIndex, axis.getAxisId());
    *separationScore = -1.0;
}

#endif //KDTREE_AXISSCORER_H
