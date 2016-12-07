#ifndef KDTREE_BINARYTREE_H
#define KDTREE_BINARYTREE_H

#include <vector>
#include "DimensionSelector.h"
#include "SplitGenerator.h"
#include "memory"
#include <string>

using namespace std;

enum NodeType {
    EMPTY,
    INTERNAL,
    LEAF
};

template <typename DataType, typename DimensionType>
class Node {
public:
    Node(NodeType type, DimensionType id) : type(type), id(id) {}
    Node(NodeType type, DimensionType id, DimensionType sampleId) : type(type), id(id), sampleId(sampleId) {}
    Node(NodeType type, DimensionType id, const Split<DataType, DimensionType> &_split) : type(type), id(id) {
       split =
           unique_ptr<Split<DataType, DimensionType>> (
               new Split<DataType, DimensionType>(
                   _split.getSplitDimension(),
                   _split.getSplitPoint(),
                   _split.getLeftBounds(),
                   _split.getRightBounds()
               )
           );
    }

    NodeType getType() const { return type; }
    DimensionType getSampleId() const { return sampleId; }
    unique_ptr<Split<DataType, DimensionType>> &getSplit() { return split; }
    string toString() {
        map<string, string> m;
        m["type"] = to_string(type);
        m["id"] = to_string(id);
        m["sampleId"] = to_string(sampleId);
        if (split) {
            m["split"] = split->toString();
        } else {
            // empty_pointer
            m["split"] = "n";
        }
        return Util::convertMapToString(m, ":no:", ";no;");
    }
    static Node<DataType, DimensionType> fromString(string objectStr) {
        map<string, string> m = Util::convertStringToMap(objectStr, ":no:", ";no;");
        int typeInt = stoi(m["type"]);
        if (typeInt == 0) {
            DimensionType id = stol(m["id"]);
            return Node<DataType, DimensionType>(NodeType::EMPTY, id);
        } else if (typeInt == 1) {
            Split<DataType, DimensionType> split = Split<DataType, DimensionType>::fromString(m["split"]);
            DimensionType id = stol(m["id"]);
            return Node<DataType, DimensionType>(NodeType::INTERNAL, id, split);
        } else {
            DimensionType sampleId = stol(m["sampleId"]);
            DimensionType id = stol(m["id"]);
            return Node<DataType, DimensionType>(NodeType::LEAF, id, sampleId);
        }
    };
    DimensionType getId() const { return id; }
private:
    NodeType type;
    DimensionType id;
    DimensionType sampleId = -1;
    unique_ptr<Split<DataType, DimensionType>> split;
};

template <typename DataType, typename DimensionType>
class KDTree {
public:
    KDTree(DimensionType numNodes) : numNodes(numNodes) {
        nodes.reserve(numNodes);
        for (int id = 0; id < numNodes; id++) {
            insertEmptyNode(id);
        }
    }
    void insertEmptyNode(const DimensionType nodeId) {
        nodes[nodeId] = new Node<DataType, DimensionType>(NodeType::EMPTY, nodeId);
    }
    void insertLeafNode(const DimensionType nodeId, const DimensionType sampleId) {
        nodes[nodeId] = new Node<DataType, DimensionType>(NodeType::LEAF, nodeId, sampleId);
    }
    void insertInternalNode(
        const DimensionType nodeId,
        const Split<DataType, DimensionType> &dimensionWithSplitInfo
    ) {
        nodes[nodeId] = new Node<DataType, DimensionType>(NodeType::INTERNAL, nodeId, dimensionWithSplitInfo);
        int a = 1;
    }
    Node<DataType, DimensionType> *getNode(const DimensionType nodeId) const { return nodes[nodeId]; };
    DimensionType getNumNodes() const { return numNodes; }

    static DimensionType leftNodeId(const DimensionType nodeId) { return nodeId*2 + 1; }
    static DimensionType rightNodeId(const DimensionType nodeId) { return nodeId*2 + 2; }
    static DimensionType parentNodeId(const DimensionType nodeId) { return (nodeId-1)/2; }
    static Point<DataType, DimensionType> nearestNeighbor(
        const KDTree<DataType, DimensionType> &kdtree,
        const Point<DataType, DimensionType> &query
    );
    static DimensionType getNumNodes(const DimensionType numSamples) {
        int numLeaves = pow(2, floor(log2(numSamples)) + 1);
        return numLeaves*2 - 1;
    }
private:
    const DimensionType numNodes;
    vector<Node<DataType, DimensionType>*> nodes;
};

template <typename DataType, typename DimensionType>
Point<DataType, DimensionType> KDTree<DataType, DimensionType>::nearestNeighbor(
    const KDTree<DataType, DimensionType> &kdtree,
    const Point<DataType, DimensionType> &query
) {

    return query;
};


#endif //KDTREE_BINARYTREE_H
