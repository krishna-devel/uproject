#ifndef KDTREE_NODEBUILDER_H
#define KDTREE_NODEBUILDER_H

#include "Segment.h"
#include "KDTree.h"
#include "SegmentSplitter.h"
#include "SplitGenerator.h"
#include "tbb/parallel_do.h"

using namespace std;
using namespace tbb;

template <typename DataType, typename DimensionType>
class DataForIteration {
public:
    DataForIteration(
        const Samples<DataType> &samples,
        const SampleIdsInSegment<DimensionType> sampleIdsInSegment,
        const DimensionType nodeId,
        const DimensionSelectorType dimensionSelectorType,
        const SplittingMethod splittingMethod,
        const DimensionType lastDimensionUsedForSplitting,
        KDTree<DataType, DimensionType> * const kdtree
    ) :
        samples(samples),
        sampleIdsInSegment(sampleIdsInSegment),
        nodeId(nodeId),
        dimensionSelectorType(dimensionSelectorType),
        splittingMethod(splittingMethod),
        lastDimensionUsedForSplitting(lastDimensionUsedForSplitting),
        kdtree(kdtree) {}
    const Samples<DataType> &getSamples() const { return samples; }
    const SampleIdsInSegment<DimensionType> &getSampleIdsInSegment() const { return sampleIdsInSegment; }
    const DimensionType getNodeId() const { return nodeId; }
    const SplittingMethod getSplittingMethod() const { return splittingMethod; }
    const DimensionSelectorType getDimensionSelectorType() const { return dimensionSelectorType; }
    const DimensionType getLastDimensionUsedForSplitting() const { return lastDimensionUsedForSplitting; }
    KDTree<DataType, DimensionType> *getKdtree() const { return kdtree; }
private:
    const Samples<DataType> &samples;
    SampleIdsInSegment<DimensionType> sampleIdsInSegment;
    DimensionType nodeId;
    DimensionSelectorType dimensionSelectorType;
    SplittingMethod splittingMethod;
    DimensionType lastDimensionUsedForSplitting;
    KDTree<DataType, DimensionType> * const kdtree;
};

template <typename DataType, typename DimensionType>
class NodeBuilder {
public:
    /**
     * This method is used to build a node in kd-tree.
     *
     * @param params
     * @param segment
     * @param kdtree
     */
    static void build(const DataForIteration<DataType, DimensionType> &dataForIteration);
    static void buildNonRecursive(const DataForIteration<DataType, DimensionType> &dataForIteration);
};

template <typename DataType, typename DimensionType>
void NodeBuilder<DataType, DimensionType>::build(const DataForIteration<DataType, DimensionType> &dataForIteration) {
    const Samples<DataType> &samples = dataForIteration.getSamples();
    KDTree<DataType, DimensionType> * const kdtree = dataForIteration.getKdtree();

    DimensionType nodeId = dataForIteration.getNodeId();
    const Segment<DataType, DimensionType> segment (samples, dataForIteration.getSampleIdsInSegment());
    DimensionType numRowsInSegment = segment.getSampleIdsInSegment().size();

    if(numRowsInSegment == 1) {
        // Segment has just one sample. So insert a leaf node in the tree.
        kdtree->insertLeafNode(nodeId, segment.getSampleIdsInSegment()[0]);
    } else {
        // Segment has more than one sample. So split it almost equally and insert a split node in the tree.

        // Select the dimension that will be used to split the node
        DimensionType dimensionToSplitBy =
            DimensionSelectorByType<DataType, DimensionType>::get(
                segment,
                dataForIteration.getDimensionSelectorType(),
                dataForIteration.getLastDimensionUsedForSplitting()
            );

        // This step does the following steps:
        // - Determines the split point that is used to generate 2 new segments.
        // - Determines the bounds for the 2 segments
        SplitWithSegments<DataType, DimensionType> splitWithSegments =
            SplitGenerator<DataType, DimensionType>::generate(
                segment,
                dataForIteration.getSplittingMethod(),
                dimensionToSplitBy
            );

        Split<DataType, DimensionType> split = splitWithSegments.getSplit();
        SplitSegments<DataType, DimensionType> splitSegments = splitWithSegments.getSplitSegments();

        // Insert the node built in this step and then build left and right child nodes.
        kdtree->insertInternalNode(nodeId, split);
        build(DataForIteration<DataType, DimensionType>(
            samples,
            splitSegments.getSegmentLessThanThreshold().getSampleIdsInSegment(),
            KDTree<DataType, DimensionType>::leftNodeId(nodeId),
            dataForIteration.getDimensionSelectorType(),
            dataForIteration.getSplittingMethod(),
            dimensionToSplitBy,
            kdtree
        ));
        build(DataForIteration<DataType, DimensionType>(
            samples,
            splitSegments.getSegmentGreaterThanThreshold().getSampleIdsInSegment(),
            KDTree<DataType, DimensionType>::rightNodeId(nodeId),
            dataForIteration.getDimensionSelectorType(),
            dataForIteration.getSplittingMethod(),
            dimensionToSplitBy,
            kdtree
        ));
    }
};

template <typename DataType, typename DimensionType>
class ParallelNodeBuilder {
public:
    void operator()(
        const DataForIteration<DataType, DimensionType> &dataForIteration,
        parallel_do_feeder<DataForIteration<DataType, DimensionType>>& feeder
    ) const {

        const Samples<DataType> &samples = dataForIteration.getSamples();
        KDTree<DataType, DimensionType> * const kdtree = dataForIteration.getKdtree();

        DimensionType nodeId = dataForIteration.getNodeId();
        const Segment<DataType, DimensionType> segment (samples, dataForIteration.getSampleIdsInSegment());
        DimensionType numRowsInSegment = segment.getSampleIdsInSegment().size();

        if(numRowsInSegment == 1) {
            // Segment has just one sample. So insert a leaf node in the tree.
            kdtree->insertLeafNode(nodeId, segment.getSampleIdsInSegment()[0]);
        } else {
            // Segment has more than one sample. So split it almost equally and insert a split node in the tree.

            // Select the dimension that will be used to split the node
            DimensionType dimensionToSplitBy =
                DimensionSelectorByType<DataType, DimensionType>::get(
                    segment,
                    dataForIteration.getDimensionSelectorType(),
                    dataForIteration.getLastDimensionUsedForSplitting()
                );

            // This step does the following steps:
            // - Determines the split point that is used to generate 2 new segments.
            // - Determines the bounds for the 2 segments
            SplitWithSegments<DataType, DimensionType> splitWithSegments =
                SplitGenerator<DataType, DimensionType>::generate(
                    segment,
                    dataForIteration.getSplittingMethod(),
                    dimensionToSplitBy
                );

            Split<DataType, DimensionType> split = splitWithSegments.getSplit();
            SplitSegments<DataType, DimensionType> splitSegments = splitWithSegments.getSplitSegments();

            // Insert the node built in this step and then build left and right child nodes.
            kdtree->insertInternalNode(nodeId, split);

            feeder.add(DataForIteration<DataType, DimensionType>(
                samples,
                splitSegments.getSegmentLessThanThreshold().getSampleIdsInSegment(),
                KDTree<DataType, DimensionType>::leftNodeId(nodeId),
                dataForIteration.getDimensionSelectorType(),
                dataForIteration.getSplittingMethod(),
                dimensionToSplitBy,
                kdtree
            ));
            feeder.add(DataForIteration<DataType, DimensionType>(
                samples,
                splitSegments.getSegmentGreaterThanThreshold().getSampleIdsInSegment(),
                KDTree<DataType, DimensionType>::rightNodeId(nodeId),
                dataForIteration.getDimensionSelectorType(),
                dataForIteration.getSplittingMethod(),
                dimensionToSplitBy,
                kdtree
            ));
        }

    }
};

template <typename DataType, typename DimensionType>
void NodeBuilder<DataType, DimensionType>::buildNonRecursive(
    const DataForIteration<DataType, DimensionType> &dataForIteration
) {
    vector<DataForIteration<DataType, DimensionType>> list {dataForIteration};
    parallel_do( list.begin(), list.end(), ParallelNodeBuilder<DataType, DimensionType>() );
}

#endif //KDTREE_NODEBUILDER_H
