#ifndef KDTREE_BINARYTREE_H
#define KDTREE_BINARYTREE_H

#include <vector>
#include "DimensionSelector.h"
#include "SplitGenerator.h"
#include "memory"
#include <string>
//#include "tbb/tbb.h"
//#include "tbb/parallel_do.h"

using namespace std;
//using namespace tbb;

enum NodeType {
    INTERNAL,
    LEAF
};

template <typename DataType, typename DimensionType>
class Node {
public:
    Node(NodeType type, DimensionType sampleId) : type(type), sampleId(sampleId) {}
    Node(NodeType type, const Split<DataType, DimensionType> &_split) : type(type) {
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
            Split<DataType, DimensionType> split = Split<DataType, DimensionType>::fromString(m["split"]);
            return Node<DataType, DimensionType>(NodeType::INTERNAL, split);
        } else {
            DimensionType sampleId = stol(m["sampleId"]);
            return Node<DataType, DimensionType>(NodeType::LEAF, sampleId);
        }
    };
private:
    NodeType type;
    DimensionType sampleId = -1;
    unique_ptr<Split<DataType, DimensionType>> split;
};

template <typename DataType, typename DimensionType>
class KDTree {
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
