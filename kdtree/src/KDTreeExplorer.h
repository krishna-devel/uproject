#ifndef KDTREE_KDTREEEXPLORER_H
#define KDTREE_KDTREEEXPLORER_H

#include "memory"
#include "KDTree.h"
#include "tbb/tbb.h"
#include "tbb/parallel_do.h"
#include <stack>

using namespace std;
using namespace tbb;

template <typename DataType, typename DimensionType>
class DataToSearchNodes {
public:
    DataToSearchNodes(
        const KDTree<DataType, DimensionType> &kdtree,
        const Point<DataType, DimensionType> &query,
        const DimensionType nodeId,
        concurrent_vector<DimensionType> *potentialNeighbors
    ) : kdtree(kdtree), query(query), nodeId(nodeId), potentialNeighbors(potentialNeighbors) {}
    const KDTree<DataType, DimensionType> &getKdtree() const { return kdtree; }
    const Point<DataType, DimensionType> &getQuery() const { return query; }
    const DimensionType getNodeId() const { return nodeId; }
    concurrent_vector<DimensionType, cache_aligned_allocator<DimensionType>> *getPotentialNeighbors() const {
        return potentialNeighbors;
    }
private:
    const KDTree<DataType, DimensionType> &kdtree;
    const Point<DataType, DimensionType> &query;
    const DimensionType nodeId;
    concurrent_vector<DimensionType> *potentialNeighbors;
};

template <typename DataType, typename DimensionType>
class NearestNeighbor {
public:
    NearestNeighbor(
        const vector<DataType> &coefficients,
        const DataType distance
    ) : distance(distance) {
        point = unique_ptr<Point<DataType, DimensionType>> (new Point<DataType, DimensionType>(coefficients));
    }
    const unique_ptr<Point<DataType, DimensionType>> &getPoint() { return point; }
    const DataType getDistance() const { return distance; }
private:
    unique_ptr<Point<DataType, DimensionType>> point;
    const DataType distance;
};

template <typename DataType, typename DimensionType>
class NearestNeighbor1 {
public:
    NearestNeighbor1(
        const Point<DataType, DimensionType> &point,
        const DataType distance) : point(point), distance(distance) {}
    const Point<DataType, DimensionType> &getPoint() const { return point; }
    const DataType getDistance() const { return distance; }
private:
    Point<DataType, DimensionType> point;
    const DataType distance;
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
public:
    vector<DimensionType> getNodesToSearch(const DataToSearchNodes<DataType, DimensionType> &dataToSearchNodes) const {
        vector<DimensionType> nodesToExplore;

        const KDTree<DataType, DimensionType> &kdTree = dataToSearchNodes.getKdtree();
        const DimensionType nodeId = dataToSearchNodes.getNodeId();
        const Point<DataType, DimensionType> &query = dataToSearchNodes.getQuery();
        concurrent_vector<DimensionType> *potentialNeighbors = dataToSearchNodes.getPotentialNeighbors();
        Node<DataType, DimensionType> *node = kdTree.getNode(nodeId);

        if (node->getType() == NodeType::LEAF) {
            potentialNeighbors->push_back(node->getSampleId());
        } else {
            // Make decision based on split threshold
            DimensionType splitDimension = node->getSplit()->getSplitDimension();
            DataType splitThreshold = node->getSplit()->getSplitThreshold();
            bool shouldMoveLeftBySplit = query.getCoefficients()[splitDimension] < splitThreshold;

            // Make decision based on bounds
            DataType distanceFromLeft = node->getSplit()->getLeftBounds().distanceFromPoint(query);
            DataType distanceFromRight = node->getSplit()->getRightBounds().distanceFromPoint(query);

            if (shouldMoveLeftBySplit) {
                nodesToExplore.push_back(kdTree.leftNodeId(nodeId));
            } else {
                nodesToExplore.push_back(kdTree.rightNodeId(nodeId));
            }

            if (shouldMoveLeftBySplit && distanceFromRight <= distanceFromLeft) {
                nodesToExplore.push_back(kdTree.rightNodeId(nodeId));
            } else if (!shouldMoveLeftBySplit && distanceFromRight >= distanceFromLeft) {
                nodesToExplore.push_back(kdTree.leftNodeId(nodeId));
            }
        }
        return nodesToExplore;
    }

    void operator()(
        const DataToSearchNodes<DataType, DimensionType> &dataToSearchNodes,
        parallel_do_feeder<DataToSearchNodes<DataType, DimensionType>> &feeder
    ) const {
        for (DimensionType nodeId: getNodesToSearch(dataToSearchNodes)) {
            DataToSearchNodes<DataType, DimensionType> nextDataToSearchNodes(
                dataToSearchNodes.getKdtree(),
                dataToSearchNodes.getQuery(),
                nodeId,
                dataToSearchNodes.getPotentialNeighbors()
            );
            feeder.add(nextDataToSearchNodes);
        }
    }

    static NearestNeighbor<DataType, DimensionType> *getNearestNeighbor(
        const Samples<DataType> &samples,
        const Point<DataType, DimensionType> &query,
        concurrent_vector<DimensionType> potentialNeighbors
    ) {
        vector<DimensionType> sampleIdsInSegment;
        for (DimensionType id : potentialNeighbors) {
            sampleIdsInSegment.push_back(id);
        }
        Segment<DataType, DimensionType> segment (samples, sampleIdsInSegment);

        Point<DataType, DimensionType> currentNearestNeighbor = segment.getPoint(sampleIdsInSegment[0]);
        DataType currentLowestDistance = numeric_limits<double>::infinity();
        for (DimensionType id: sampleIdsInSegment) {
            Point<DataType, DimensionType> neighbor = segment.getPoint(id);
            DataType distance = Point<DataType, DimensionType>::euclideanDistance(query, neighbor);
            if (distance < currentLowestDistance) {
                currentNearestNeighbor = neighbor;
                currentLowestDistance = distance;
            }
        }

        return new NearestNeighbor<DataType, DimensionType>(currentNearestNeighbor.getCoefficients(), currentLowestDistance);
    }

    static NearestNeighbor<DataType, DimensionType> *findNearestNeighbor(
        const Samples<DataType> &samples,
        const KDTree<DataType, DimensionType> &kdTree,
        const Point<DataType, DimensionType> &query
    ) {
        concurrent_vector<DimensionType> potentialNeighbors;
        DataToSearchNodes<DataType, DimensionType> dataToSearchNodes (kdTree, query, 0, &potentialNeighbors);
        vector<DataToSearchNodes<DataType, DimensionType>> list {dataToSearchNodes};
        parallel_do( list.begin(), list.end(), ParallelNodeExplorer<DataType, DimensionType>() );
        return getNearestNeighbor(samples, query, potentialNeighbors);
    }


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

    static NearestNeighbor1<DataType, DimensionType> *findNearestNeighbor1(
        const Samples<DataType> &samples,
        const KDTree<DataType, DimensionType> &kdTree,
        const Point<DataType, DimensionType> &query
    ) {
        Segment<DataType, DimensionType> segment(samples);

        NearestNeighbor1<DataType, DimensionType> *currentNearestNeighbor =
            new NearestNeighbor1<DataType, DimensionType>(segment.getPoint(0), numeric_limits<double>::infinity());

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
//                DataType distanceFromNode = Point<DataType, DimensionType>::squaredDistance(point, query);
                if (distanceFromNodeToExplore < currentNearestNeighbor->getDistance()) {
                    currentNearestNeighbor =
                        new NearestNeighbor1<DataType, DimensionType>(point, distanceFromNodeToExplore);
                }
            } else {
                const DimensionType leftNodeId = kdTree.leftNodeId(nodeId);
                NodeToExplore<DataType, DimensionType> *leftNodeToExplore =
                    getNodeToExplore(segment, leftNodeId, kdTree.getNode(leftNodeId), query);

                const DimensionType rightNodeId = kdTree.rightNodeId(nodeId);
                NodeToExplore<DataType, DimensionType> *rightNodeToExplore =
                    getNodeToExplore(segment, rightNodeId, kdTree.getNode(rightNodeId), query);

                if (leftNodeToExplore->getDistanceFromNode() <= currentNearestNeighbor->getDistance()) {
                    nodesToExplore.push(leftNodeToExplore);
                }
                if (rightNodeToExplore->getDistanceFromNode() <= currentNearestNeighbor->getDistance()) {
                    nodesToExplore.push(rightNodeToExplore);
                }
            }
        }

        return currentNearestNeighbor;
    };

};

#endif //KDTREE_KDTREEEXPLORER_H
