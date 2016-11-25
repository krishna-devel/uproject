//#ifndef KDTREE_NODEBUILDER_H
//#define KDTREE_NODEBUILDER_H
//
//#include "Node.h"
//#include "Segment.h"
//#include "KDTree.h"
//
//template <typename DataType, typename DimensionType>
//class NodeBuilder {
//public:
//    static void build(
//        const Segment<DataType, DimensionType> &segment,
//        const long nodeId,
//        KDTree<DataType, DimensionType> &kdtree
//    );
//};
//
//template <typename DataType, typename DimensionType>
//void NodeBuilder<DataType, DimensionType>::build(
//    const Segment<DataType, DimensionType> &segment,
//    const long nodeId,
//    KDTree<DataType, DimensionType> *kdtree
//) {
//    DimensionType numRowsInSegment = segment.getSampleIdsInSegment().size();
//    if(numRowsInSegment == 1) {
//        kdtree->insertInternalNode(nodeId, segment.getSampleIdsInSegment()[0]);
////        Node<DataType, DimensionType> node (NodeType::LEAF, segment.getSampleIdsInSegment()[0]);
////        kdtree[nodeId] = node;
//    }
//
//}
//
//#endif //KDTREE_NODEBUILDER_H
