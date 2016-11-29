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
        const DimensionType nodeId,
        const DimensionSelectorType dimensionSelectorType,
        const SplittingMethod splittingMethod,
        const DimensionType lastDimensionUsedForSplitting
    ) :
        nodeId(nodeId),
        dimensionSelectorType(dimensionSelectorType),
        splittingMethod(splittingMethod),
        lastDimensionUsedForSplitting(lastDimensionUsedForSplitting) {
        dimensionSelector = unique_ptr<DimensionSelector<DataType, DimensionType>>(
            DimensionSelectorByType<DataType, DimensionType>::get(dimensionSelectorType, lastDimensionUsedForSplitting)
        );
    }
    const DimensionType getNodeId() const { return nodeId; }
    const SplittingMethod getSplittingMethod() const { return splittingMethod; }
    unique_ptr<DimensionSelector<DataType, DimensionType>> &getDimensionSelector() { return dimensionSelector; }

    unique_ptr<NodeBuilderParams<DataType, DimensionType>> getParamsForLeftNode(
        const DimensionType lastDimensionUsedForSplitting
    ) {
        return unique_ptr<NodeBuilderParams<DataType, DimensionType>>(new NodeBuilderParams(
            KDTree<DataType, DimensionType>::leftNodeId(nodeId),
            dimensionSelectorType,
            splittingMethod,
            lastDimensionUsedForSplitting
        ));
    };

    unique_ptr<NodeBuilderParams<DataType, DimensionType>> getParamsForRightNode(
        const DimensionType lastDimensionUsedForSplitting
    ) {
        return unique_ptr<NodeBuilderParams<DataType, DimensionType>>(new NodeBuilderParams(
            KDTree<DataType, DimensionType>::rightNodeId(nodeId),
            dimensionSelectorType,
            splittingMethod,
            lastDimensionUsedForSplitting
        ));
    };

private:
    const DimensionType nodeId;
    const DimensionSelectorType dimensionSelectorType;
    const SplittingMethod splittingMethod;
    const DimensionType lastDimensionUsedForSplitting;
    unique_ptr<DimensionSelector<DataType, DimensionType>> dimensionSelector;
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
        const unique_ptr<NodeBuilderParams<DataType, DimensionType>> &params,
        const Segment<DataType, DimensionType> &segment,
        KDTree<DataType, DimensionType> *kdtree
    );
};

template <typename DataType, typename DimensionType>
void NodeBuilder<DataType, DimensionType>::build(
    const unique_ptr<NodeBuilderParams<DataType, DimensionType>> &params,
    const Segment<DataType, DimensionType> &segment,
    KDTree<DataType, DimensionType> *kdtree
) {
    DimensionType nodeId = params->getNodeId();
    DimensionType numRowsInSegment = segment.getSampleIdsInSegment().size();

    if(numRowsInSegment == 1) {
        // Segment has just one sample. So insert a leaf node in the tree.
        kdtree->insertLeafNode(nodeId, segment.getSampleIdsInSegment()[0]);
    } else {
        // Segment has more than one sample. So split it almost equally and insert a split node in the tree.

        // Select the dimension that will be used to split the node
        DimensionType dimensionToSplitBy = params->getDimensionSelector()->getNextDimension(segment);

        // This step does the following steps:
        // - Determines the split point that is used to generate 2 new segments.
        // - Determines the bounds for the 2 segments
        SplitWithSegments<DataType, DimensionType> splitWithSegments =
            SplitGenerator<DataType, DimensionType>::generate(
                segment,
                params->getSplittingMethod(),
                dimensionToSplitBy
            );

        Split<DataType, DimensionType> split = splitWithSegments.getSplit();
        SplitSegments<DataType, DimensionType> splitSegments = splitWithSegments.getSplitSegments();

        // Insert the node built in this step and then build left and right child nodes.
        kdtree->insertInternalNode(nodeId, split);
        build(
            params->getParamsForLeftNode(dimensionToSplitBy),
            splitSegments.getSegmentLessThanThreshold(),
            kdtree
        );
        build(
            params->getParamsForRightNode(dimensionToSplitBy),
            splitSegments.getSegmentGreaterThanThreshold(),
            kdtree
        );
    }

}

#endif //KDTREE_NODEBUILDER_H
