#ifndef KDTREE_KDTREEHANDLER_H
#define KDTREE_KDTREEHANDLER_H

#include "KDTreeIO.h"
#include "KDTreeBuilder.h"
#include "KDTreeExplorer.h"

template <typename DataType, typename DimensionType>
class KDTreeHandler {
public:
    /**
     * This method is used to build the KD-tree.
     *
     * @param inputSampleDataset
     * @param outputModelFilename
     * @param dimensionSelectorType
     * @param splittingMethod
     * @param parallelExecution
     */
    static void buildKDTree(
        const string &inputSampleDataset,
        const string &outputModelFilename,
        const DimensionSelectorType &dimensionSelectorType,
        const SplittingMethod &splittingMethod,
        const bool &parallelExecution
    );

    /**
     * This method is used to search for queries in a pre-built KD-tree
     * that was built in the buildKDTree method.
     *
     * @param inputSampleDataset
     * @param modelFilename
     * @param queriesFileName
     * @param nnOuputDataset
     */
    static void queryKDTree(
        const string &inputSampleDataset,
        const string &modelFilename,
        const string &queriesFileName,
        const string &nnOuputDataset
    );
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

template <typename DataType, typename DimensionType>
void KDTreeHandler<DataType, DimensionType>::queryKDTree(
    const string &inputSampleDataset, 
    const string &modelFilename,
    const string &queriesFileName, 
    const string &nnOuputDataset
) {
    Samples<DataType> samples = KDTreeIO<DataType, DimensionType>::loadSamples(inputSampleDataset);
    KDTree<DataType, DimensionType> loadedKDTree = KDTreeIO<DataType, DimensionType>::load(modelFilename);

    Samples<DataType> queries = KDTreeIO<DataType, DimensionType>::loadSamples(queriesFileName);
    DimensionType numQueries = queries.rows();
    vector<DimensionType> queryIdsInSegment(numQueries);
    iota(begin(queryIdsInSegment), end(queryIdsInSegment), 0);

    Segment<DataType, DimensionType> samplesSegment(samples);
    Segment<DataType, DimensionType> queriesSegment(queries);

    vector<string> nearestNeighborsWithDistance;
    for (DimensionType queryId : queryIdsInSegment) {
        NearestNeighbor<DataType, DimensionType> *nearestNeighbor =
            KDTreeExplorer<DataType, DimensionType>::findNearestNeighbor(
                samples,
                loadedKDTree,
                queriesSegment.getPoint(queryId)
            );

        string nn = "" + to_string(nearestNeighbor->getSampleId()) + "," + to_string(nearestNeighbor->getEuclideanDistance());
        nearestNeighborsWithDistance.push_back(nn);
    }

    Util::writeStringVector(nnOuputDataset, nearestNeighborsWithDistance);

}

#endif //KDTREE_KDTREEHANDLER_H
