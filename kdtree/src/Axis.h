#ifndef KDTREE_AXIS_H
#define KDTREE_AXIS_H

#include <vector>
#include "MatrixXdAdapter.h"

using namespace std;

template <typename T>
class Axis {
public:
    // constructor
    Axis(const MatrixXdAdapter<T> &adapter, int axisId) : adapter(adapter), axisId(axisId) {}

    // getters
    const MatrixXdAdapter<T> &getAdapter() const { return adapter; }
    int getAxisId() const { return axisId; }

    vector<T> getValuesAlongAxis();


private:
    const MatrixXdAdapter<T> &adapter;
    int axisId;

};

template <typename T>
vector<T> Axis<T>::getValuesAlongAxis() {
    vector<T> vectorToReturn;
    const Matrix<T, Dynamic, Dynamic> &samples = adapter.getSamples();
    const vector<int> &indices = adapter.getRowIndices();

    if (axisId < samples.cols()) {
        for (int index: indices) {
            vectorToReturn.push_back(samples(index, axisId));
        }
    }
    return vectorToReturn;
}

#endif //KDTREE_AXIS_H
