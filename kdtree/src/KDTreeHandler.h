#ifndef KDTREE_KDTREEHANDLER_H
#define KDTREE_KDTREEHANDLER_H

#include "KDTreeIO.h"
#include "KDTreeBuilder.h"

template <typename DataType, typename DimensionType>
class KDTreeHandler {
public:
    static void buildKDTree(
        const string &inputSampleDataset,
        const string &outputModelFilename,
        const DimensionSelectorType &dimensionSelectorType,
        const SplittingMethod &splittingMethod,
        const bool &parallelExecution
    );

//    static void queryKDTree();
};

template <typename DataType, typename DimensionType>
void KDTreeHandler<DataType, DimensionType>::buildKDTree(
    const string &inputSampleDataset,
    const string &outputModelFilename,
    const DimensionSelectorType &dimensionSelectorType,
    const SplittingMethod &splittingMethod,
    const bool &parallelExecution
) {

    Samples<DataType> samples = KDTreeIO<DataType, DimensionType>::loadSamples(inputSampleDataset);
    DimensionType numSamples = samples.rows();
    vector<DimensionType> sampleIdsInSegment(numSamples);
    iota(begin(sampleIdsInSegment), end(sampleIdsInSegment), 0);

    DimensionType numNodes = KDTree<DataType, DimensionType>::getNumNodes(numSamples);

    KDTree<DataType, DimensionType> kdTree(numNodes);
    DataToBuildNodes<DataType, DimensionType> dataForIteration = DataToBuildNodes<DataType, DimensionType>(
            samples,
            sampleIdsInSegment,
            0,
            dimensionSelectorType,
            splittingMethod,
            -1,
            &kdTree
    );

    if (parallelExecution) {
        KDTreeBuilder<DataType, DimensionType>::buildInParallel(dataForIteration);
    } else {
        KDTreeBuilder<DataType, DimensionType>::buildUsingRecursion(dataForIteration);
    }

    KDTreeIO<DataType, DimensionType>::write(kdTree, outputModelFilename);
}



#endif //KDTREE_KDTREEHANDLER_H
