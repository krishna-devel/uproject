#ifndef KDTREE_BINARYTREE_H
#define KDTREE_BINARYTREE_H

#include <vector>
#include "DimensionSelector.h"
#include "memory"

using namespace std;

enum NodeType {
    INTERNAL,
    LEAF
};

template <typename DataType, typename DimensionType>
class Node {
public:
    Node(NodeType type, DimensionType sampleId) : type(type), sampleId(sampleId) {}
    Node(NodeType type, const DimensionWithSplitInfo<DataType, DimensionType> &_dimensionWithSplitInfo) : type(type) {
       dimensionWithSplitInfo =
           unique_ptr<DimensionWithSplitInfo<DataType, DimensionType>> (
               new DimensionWithSplitInfo<DataType, DimensionType>(
                       _dimensionWithSplitInfo.getSplitDimension(),
                       _dimensionWithSplitInfo.getSplitInfo()
               )
           );
    }
    NodeType getType() const { return type; }
    DimensionType getSampleId() const { return sampleId; }
    unique_ptr<DimensionWithSplitInfo<DataType, DimensionType>> &getDimensionWithSplitInfo() {
        return dimensionWithSplitInfo;
    }
private:
    NodeType type;
    DimensionType sampleId = -1;
    unique_ptr<DimensionWithSplitInfo<DataType, DimensionType>> dimensionWithSplitInfo;
};

template <typename DataType, typename DimensionType>
class KDTree {
private:
    DimensionType numNodes;
    vector<Node<DataType, DimensionType>*> nodes;
public:
    KDTree(DimensionType numNodes) : numNodes(numNodes) { nodes.reserve(numNodes); }
    void insertLeafNode(const DimensionType nodeId, const DimensionType sampleId) {
        nodes[nodeId] = new Node<DataType, DimensionType>(NodeType::LEAF, sampleId);
    }
    void insertInternalNode(
        const DimensionType nodeId,
        const DimensionWithSplitInfo<DataType, DimensionType> &dimensionWithSplitInfo
    ) {
        nodes[nodeId] = new Node<DataType, DimensionType>(NodeType::INTERNAL, dimensionWithSplitInfo);
    }
    Node<DataType, DimensionType> *getNode(const DimensionType nodeId) { return nodes[nodeId]; };
    static DimensionType leftNodeId(const DimensionType nodeId) { return nodeId*2 + 1; }
    static DimensionType rightNodeId(const DimensionType nodeId) { return nodeId*2 + 2; }
    static DimensionType parentNodeId(const DimensionType nodeId) { return (nodeId-1)/2; }
};


#endif //KDTREE_BINARYTREE_H
