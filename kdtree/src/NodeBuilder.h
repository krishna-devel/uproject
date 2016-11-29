#ifndef KDTREE_NODEBUILDER_H
#define KDTREE_NODEBUILDER_H

#include "Segment.h"
#include "KDTree.h"
#include "SegmentSplitter.h"

template <typename  DataType, typename DimensionType>
class NodeBuilderParams {
public:
    NodeBuilderParams(
        const DimensionType nodeId,
        const DimensionSelectorType dimensionSelectorType,
        const DimensionSplittingMethod dimensionSplittingMethod,
        const DimensionType lastDimensionUsedForSplitting
    ) :
        nodeId(nodeId),
        dimensionSelectorType(dimensionSelectorType),
        dimensionSplittingMethod(dimensionSplittingMethod),
        lastDimensionUsedForSplitting(lastDimensionUsedForSplitting) {
        dimensionSelector = unique_ptr<DimensionSelector<DataType, DimensionType>>(
            DimensionSelectorByType<DataType, DimensionType>::get(dimensionSelectorType, lastDimensionUsedForSplitting)
        );
    }
    const DimensionType getNodeId() const { return nodeId; }
    const DimensionSplittingMethod getDimensionSplittingMethod() const { return dimensionSplittingMethod; }
    unique_ptr<DimensionSelector<DataType, DimensionType>> &getDimensionSelector() { return dimensionSelector; }

    unique_ptr<NodeBuilderParams<DataType, DimensionType>> getParamsForLeftNode(
        const DimensionType lastDimensionUsedForSplitting
    ) {
        return unique_ptr<NodeBuilderParams<DataType, DimensionType>>(new NodeBuilderParams(
                KDTree<DataType, DimensionType>::leftNodeId(nodeId),
                dimensionSelectorType,
                dimensionSplittingMethod,
                lastDimensionUsedForSplitting
        ));
    };

    unique_ptr<NodeBuilderParams<DataType, DimensionType>> getParamsForRightNode(
        const DimensionType lastDimensionUsedForSplitting
    ) {
        return unique_ptr<NodeBuilderParams<DataType, DimensionType>>(new NodeBuilderParams(
                KDTree<DataType, DimensionType>::rightNodeId(nodeId),
                dimensionSelectorType,
                dimensionSplittingMethod,
                lastDimensionUsedForSplitting
        ));
    };

private:
    const DimensionType nodeId;
    const DimensionSelectorType dimensionSelectorType;
    const DimensionSplittingMethod dimensionSplittingMethod;
    const DimensionType lastDimensionUsedForSplitting;
    unique_ptr<DimensionSelector<DataType, DimensionType>> dimensionSelector;
};

template <typename DataType, typename DimensionType>
class NodeBuilder {
public:
    static void build(
        const unique_ptr<NodeBuilderParams<DataType, DimensionType>> &params,
        const Segment<DataType, DimensionType> &segment,
        KDTree<DataType, DimensionType> *kdtree
    );
    static unique_ptr<SegmentSplitter<DataType, DimensionType>> segmentSplitter();
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
        DimensionWithSplitInfo<DataType, DimensionType>  dimensionWithSplitInfo =
                params->getDimensionSelector()->getNextDimensionToSplit(segment, params->getDimensionSplittingMethod());

        SplitSegments<DataType, DimensionType> splitSegments =
                segmentSplitter()->split(segment, dimensionWithSplitInfo);

        DimensionType dimensionUsedForSplitting = dimensionWithSplitInfo.getSplitDimension();
        kdtree->insertInternalNode(nodeId, dimensionWithSplitInfo);
        build(
                params->getParamsForLeftNode(dimensionUsedForSplitting),
                splitSegments.getSegmentLessThanThreshold(),
                kdtree
        );
        build(
                params->getParamsForRightNode(dimensionUsedForSplitting),
                splitSegments.getSegmentGreaterThanThreshold(),
                kdtree
        );
    }

}

template <typename DataType, typename DimensionType>
unique_ptr<SegmentSplitter<DataType, DimensionType>> NodeBuilder<DataType, DimensionType>::segmentSplitter() {
    return unique_ptr<SegmentSplitter<DataType, DimensionType>>(new SegmentSplitter<DataType, DimensionType>());
}

#endif //KDTREE_NODEBUILDER_H
