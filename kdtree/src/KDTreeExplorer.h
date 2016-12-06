#ifndef KDTREE_KDTREEEXPLORER_H
#define KDTREE_KDTREEEXPLORER_H

#include "memory"
#include "KDTree.h"
#include <stack>
#include <math.h>

using namespace std;

template <typename DataType, typename DimensionType>
class NearestNeighbor {
public:
    NearestNeighbor(
        const Point<DataType, DimensionType> &point,
        const DataType distance
    ) : point(point), squaredDistance(distance) {}
    const Point<DataType, DimensionType> &getPoint() const { return point; }
    const DataType getSquaredDistance() const { return squaredDistance; }
    const DataType getEuclideanDistance() const { return sqrt(squaredDistance); }
private:
    Point<DataType, DimensionType> point;
    const DataType squaredDistance;
};

template <typename DataType, typename DimensionType>
class NodeToExplore {
public:
    NodeToExplore(
        const DimensionType nodeId,
        const Node<DataType, DimensionType> *node,
        const DataType distanceFromNode) : nodeId(nodeId), node(node), distanceFromNode(distanceFromNode) {}
    const DimensionType getNodeId() const { return nodeId; }
    const Node<DataType, DimensionType> *getNode() const { return node; }
    const DataType getDistanceFromNode() const { return distanceFromNode; }
private:
    const DimensionType nodeId;
    const Node<DataType, DimensionType> *node;
    const DataType distanceFromNode;
};

template <typename DataType, typename DimensionType>
class ParallelNodeExplorer {
private:
    static NodeToExplore<DataType, DimensionType> *getNodeToExplore(
        const Segment<DataType, DimensionType> &segment,
        const DimensionType nodeId,
        Node<DataType, DimensionType> *node,
        const Point<DataType, DimensionType> &query
    ) {
        DataType distance = 0;
        if (node->getType() == NodeType::INTERNAL) {
            Bounds<DataType, DimensionType> nodeBounds(
                node->getSplit()->getLeftBounds().getMinPoint().getCoefficients(),
                node->getSplit()->getLeftBounds().getMaxPoint().getCoefficients()
            );
            distance = nodeBounds.distanceFromPoint(query);
        } else {
            distance = Point<DataType, DimensionType>::squaredDistance(segment.getPoint(node->getSampleId()), query);
        }
        return new NodeToExplore<DataType, DimensionType>(nodeId, node, distance);
    }
public:
    static NearestNeighbor<DataType, DimensionType> *findNearestNeighbor(
        const Samples<DataType> &samples,
        const KDTree<DataType, DimensionType> &kdTree,
        const Point<DataType, DimensionType> &query
    ) {
        Segment<DataType, DimensionType> segment(samples);

        NearestNeighbor<DataType, DimensionType> *currentNearestNeighbor =
            new NearestNeighbor<DataType, DimensionType>(segment.getPoint(0), numeric_limits<double>::infinity());

        stack<NodeToExplore<DataType, DimensionType>*> nodesToExplore;
        nodesToExplore.push(getNodeToExplore(segment, 0, kdTree.getNode(0), query));

        while (!nodesToExplore.empty()) {
            NodeToExplore<DataType, DimensionType> *nodeToExplore = nodesToExplore.top();
            nodesToExplore.pop();

            const Node<DataType, DimensionType> *node = nodeToExplore->getNode();
            const DimensionType nodeId = nodeToExplore->getNodeId();

            if (node->getType() == NodeType::LEAF) {
                Point<DataType, DimensionType> point = segment.getPoint(node->getSampleId());
                const DataType distanceFromNodeToExplore = nodeToExplore->getDistanceFromNode();
                if (distanceFromNodeToExplore < currentNearestNeighbor->getSquaredDistance()) {
                    currentNearestNeighbor =
                        new NearestNeighbor<DataType, DimensionType>(point, distanceFromNodeToExplore);
                }
            } else {
                const DimensionType leftNodeId = kdTree.leftNodeId(nodeId);
                NodeToExplore<DataType, DimensionType> *leftNodeToExplore =
                    getNodeToExplore(segment, leftNodeId, kdTree.getNode(leftNodeId), query);

                const DimensionType rightNodeId = kdTree.rightNodeId(nodeId);
                NodeToExplore<DataType, DimensionType> *rightNodeToExplore =
                    getNodeToExplore(segment, rightNodeId, kdTree.getNode(rightNodeId), query);

                if (leftNodeToExplore->getDistanceFromNode() <= currentNearestNeighbor->getSquaredDistance()) {
                    nodesToExplore.push(leftNodeToExplore);
                }
                if (rightNodeToExplore->getDistanceFromNode() <= currentNearestNeighbor->getSquaredDistance()) {
                    nodesToExplore.push(rightNodeToExplore);
                }
            }
        }

        return currentNearestNeighbor;
    };

};

#endif //KDTREE_KDTREEEXPLORER_H
