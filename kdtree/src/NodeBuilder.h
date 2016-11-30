#ifndef KDTREE_NODEBUILDER_H
#define KDTREE_NODEBUILDER_H

#include "Segment.h"
#include "KDTree.h"
#include "SegmentSplitter.h"
#include "SplitGenerator.h"


template <typename  DataType, typename DimensionType>
class NodeBuilderParams {
public:
    /**
     * This object contains params that are needed to build a node.
     */
    NodeBuilderParams(
        const SampleIdsInSegment<DimensionType> sampleIdsInSegment,
        const DimensionType nodeId,
        const DimensionSelectorType dimensionSelectorType,
        const SplittingMethod splittingMethod,
        const DimensionType lastDimensionUsedForSplitting
    ) :
        sampleIdsInSegment(sampleIdsInSegment),
        nodeId(nodeId),
        dimensionSelectorType(dimensionSelectorType),
        splittingMethod(splittingMethod),
        lastDimensionUsedForSplitting(lastDimensionUsedForSplitting) {
    }
    const DimensionType getNodeId() const { return nodeId; }
    const SplittingMethod getSplittingMethod() const { return splittingMethod; }
    const SampleIdsInSegment<DimensionType> &getSampleIdsInSegment() const { return sampleIdsInSegment; }
    const DimensionSelectorType getDimensionSelectorType() const { return dimensionSelectorType; }
    const DimensionType getLastDimensionUsedForSplitting() const { return lastDimensionUsedForSplitting; }

private:
    const SampleIdsInSegment<DimensionType> sampleIdsInSegment;
    const DimensionType nodeId;
    const DimensionSelectorType dimensionSelectorType;
    const SplittingMethod splittingMethod;
    const DimensionType lastDimensionUsedForSplitting;
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
    static void build(
        const Samples<DataType> &samples,
        NodeBuilderParams<DataType, DimensionType> &params,
        KDTree<DataType, DimensionType> *kdtree
    );

    static void buildNonRecursive(
        const Samples<DataType> &samples,
        NodeBuilderParams<DataType, DimensionType> &params,
        KDTree<DataType, DimensionType> *kdtree
    );


};

template <typename DataType, typename DimensionType>
void NodeBuilder<DataType, DimensionType>::build(
    const Samples<DataType> &samples,
    NodeBuilderParams<DataType, DimensionType> &params,
    KDTree<DataType, DimensionType> * const kdtree
) {
    DimensionType nodeId = params.getNodeId();
    const Segment<DataType, DimensionType> segment (samples, params.getSampleIdsInSegment());
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
                params.getDimensionSelectorType(),
                params.getLastDimensionUsedForSplitting()
            );

        // This step does the following steps:
        // - Determines the split point that is used to generate 2 new segments.
        // - Determines the bounds for the 2 segments
        SplitWithSegments<DataType, DimensionType> splitWithSegments =
            SplitGenerator<DataType, DimensionType>::generate(
                segment,
                params.getSplittingMethod(),
                dimensionToSplitBy
            );

        Split<DataType, DimensionType> split = splitWithSegments.getSplit();
        SplitSegments<DataType, DimensionType> splitSegments = splitWithSegments.getSplitSegments();

        // Insert the node built in this step and then build left and right child nodes.
        kdtree->insertInternalNode(nodeId, split);
        NodeBuilderParams<DataType, DimensionType> leftNode(
            splitSegments.getSegmentLessThanThreshold().getSampleIdsInSegment(),
            KDTree<DataType, DimensionType>::leftNodeId(nodeId),
            params.getDimensionSelectorType(),
            params.getSplittingMethod(),
            dimensionToSplitBy
        );
        NodeBuilderParams<DataType, DimensionType> rightNode(
            splitSegments.getSegmentGreaterThanThreshold().getSampleIdsInSegment(),
            KDTree<DataType, DimensionType>::rightNodeId(nodeId),
            params.getDimensionSelectorType(),
            params.getSplittingMethod(),
            dimensionToSplitBy
        );
        build(samples, leftNode, kdtree);
        build(samples, rightNode, kdtree);

    }

}

template <typename DataType, typename DimensionType>
void NodeBuilder<DataType, DimensionType>::buildNonRecursive(
    const Samples<DataType> &samples,
    NodeBuilderParams<DataType, DimensionType> &initialParams,
    KDTree<DataType, DimensionType> *kdtree
) {

    vector<NodeBuilderParams<DataType, DimensionType>> nodesToBuild;
    nodesToBuild.push_back(initialParams);

    while(!nodesToBuild.empty()) {
        NodeBuilderParams<DataType, DimensionType> params = nodesToBuild.back();
        nodesToBuild.pop_back();
        DimensionType nodeId = params.getNodeId();
        const Segment<DataType, DimensionType> segment (samples, params.getSampleIdsInSegment());
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
                    params.getDimensionSelectorType(),
                    params.getLastDimensionUsedForSplitting()
                );

            // This step does the following steps:
            // - Determines the split point that is used to generate 2 new segments.
            // - Determines the bounds for the 2 segments
            SplitWithSegments<DataType, DimensionType> splitWithSegments =
                SplitGenerator<DataType, DimensionType>::generate(
                    segment,
                    params.getSplittingMethod(),
                    dimensionToSplitBy
                );

            Split<DataType, DimensionType> split = splitWithSegments.getSplit();
            SplitSegments<DataType, DimensionType> splitSegments = splitWithSegments.getSplitSegments();

            // Insert the node built in this step and then build left and right child nodes.
            kdtree->insertInternalNode(nodeId, split);

            NodeBuilderParams<DataType, DimensionType> leftNode (
                splitSegments.getSegmentLessThanThreshold().getSampleIdsInSegment(),
                KDTree<DataType, DimensionType>::leftNodeId(nodeId),
                params.getDimensionSelectorType(),
                params.getSplittingMethod(),
                dimensionToSplitBy
            );

            NodeBuilderParams<DataType, DimensionType> rightNode (
                splitSegments.getSegmentGreaterThanThreshold().getSampleIdsInSegment(),
                KDTree<DataType, DimensionType>::rightNodeId(nodeId),
                params.getDimensionSelectorType(),
                params.getSplittingMethod(),
                dimensionToSplitBy
            );

            nodesToBuild.push_back(rightNode);
            nodesToBuild.push_back(leftNode);
        }

    }

}

#endif //KDTREE_NODEBUILDER_H
