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
    int numNodes;
    vector<Node<DataType, DimensionType>*> nodes;
public:
    KDTree(int numNodes) : numNodes(numNodes) { nodes.reserve(numNodes); }
    void insertLeafNode(const long nodeId, DimensionType sampleId) {
        nodes[nodeId] = new Node<DataType, DimensionType>(NodeType::LEAF, sampleId);
    }
    void insertInternalNode(
        const long nodeId,
        const DimensionWithSplitInfo<DataType, DimensionType> &dimensionWithSplitInfo
    ) {
        nodes[nodeId] = new Node<DataType, DimensionType>(NodeType::INTERNAL, dimensionWithSplitInfo);
    }
    Node<DataType, DimensionType> *getNode(const DimensionType nodeId) { return nodes[nodeId]; };
    DimensionType leftNodeId(const DimensionType nodeId) { return nodeId*2 + 1; }
    DimensionType rightNodeId(const DimensionType nodeId) { return nodeId*2 + 2; }
    DimensionType parentNodeId(const DimensionType nodeId) { return (nodeId-1)/2; }
};


#endif //KDTREE_BINARYTREE_H
