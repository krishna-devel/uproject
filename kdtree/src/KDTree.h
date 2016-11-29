#ifndef KDTREE_BINARYTREE_H
#define KDTREE_BINARYTREE_H

#include <vector>
#include "DimensionSelector.h"
#include "SplitGenerator.h"
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
    Node(NodeType type, const Split<DataType, DimensionType> &_dimensionWithSplitInfo) : type(type) {
       dimensionWithSplitInfo =
           unique_ptr<Split<DataType, DimensionType>> (
               new Split<DataType, DimensionType>(
                   _dimensionWithSplitInfo.getSplitDimension(),
                   _dimensionWithSplitInfo.getSplitPoint(),
                   _dimensionWithSplitInfo.getLeftBounds(),
                   _dimensionWithSplitInfo.getRightBounds()
               )
           );
    }

    NodeType getType() const { return type; }
    DimensionType getSampleId() const { return sampleId; }
    unique_ptr<Split<DataType, DimensionType>> &getDimensionWithSplitInfo() {
        return dimensionWithSplitInfo;
    }
    string toString() {
        map<string, string> m;
        m["type"] = to_string(type);
        m["sampleId"] = to_string(sampleId);
        if (dimensionWithSplitInfo) {
            m["dimensionWithSplitInfoString"] = dimensionWithSplitInfo->toString();
        } else {
            // empty_pointer
            m["dimensionWithSplitInfoString"] = "n";
        }
        return Util::convertMapToString(m, ":no:", ";no;");
    }
    static Node<DataType, DimensionType> fromString(string objectStr) {
        map<string, string> m = Util::convertStringToMap(objectStr, ":no:", ";no;");
        int typeInt = stoi(m["type"]);
        if (typeInt == 0) {
            Split<DataType, DimensionType> dimensionWithSplitInfo =
                    Split<DataType, DimensionType>::fromString(m["dimensionWithSplitInfoString"]);
            return Node<DataType, DimensionType>(NodeType::INTERNAL, dimensionWithSplitInfo);
        } else {
            DimensionType sampleId = stol(m["sampleId"]);
            return Node<DataType, DimensionType>(NodeType::LEAF, sampleId);
        }
    };
private:
    NodeType type;
    DimensionType sampleId = -1;
    unique_ptr<Split<DataType, DimensionType>> dimensionWithSplitInfo;
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
        const Split<DataType, DimensionType> &dimensionWithSplitInfo
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
