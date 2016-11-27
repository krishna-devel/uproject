#ifndef KDTREE_BINARYTREE_H
#define KDTREE_BINARYTREE_H

#include <vector>
#include "DimensionSelector.h"
#include "memory"
#include <string>

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
    string toString() {
        string dimensionWithSplitInfoString;
        if (dimensionWithSplitInfo) {
            dimensionWithSplitInfoString = dimensionWithSplitInfo->toString();
        } else {
            // empty_pointed
            dimensionWithSplitInfoString = "n";
        }
        return to_string(type) + ";" + to_string(sampleId) + ";" + dimensionWithSplitInfoString;
    }
    static Node<DataType, DimensionType> fromString(string objectStr) {
        vector<string> data;
        istringstream ss(objectStr);
        string token;
        while(getline(ss, token, ';')) { data.push_back(token); }

        NodeType type = static_cast<NodeType>(stoi(data[0]));
        int typeInt = stoi(data[0]);
        if (typeInt == 0) {
            DimensionWithSplitInfo<DataType, DimensionType> dimensionWithSplitInfo =
                    DimensionWithSplitInfo<DataType, DimensionType>::fromString(data[2]);
            return Node<DataType, DimensionType>(NodeType::INTERNAL, dimensionWithSplitInfo);
        } else {
            DimensionType sampleId = stol(data[1]);
            return Node<DataType, DimensionType>(NodeType::LEAF, sampleId);
        }
    };
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
    Node<DataType, DimensionType> *getNode(const DimensionType nodeId) const { return nodes[nodeId]; };
    DimensionType getNumNodes() const { return numNodes; }

    static DimensionType leftNodeId(const DimensionType nodeId) { return nodeId*2 + 1; }
    static DimensionType rightNodeId(const DimensionType nodeId) { return nodeId*2 + 2; }
    static DimensionType parentNodeId(const DimensionType nodeId) { return (nodeId-1)/2; }
};


#endif //KDTREE_BINARYTREE_H
