#ifndef KDTREE_KDTREEEXPLORER_H
#define KDTREE_KDTREEEXPLORER_H

#include "memory"
#include "KDTree.h"
#include "tbb/tbb.h"
#include "tbb/parallel_do.h"

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

    static NearestNeighbor<DataType, DimensionType>  *getNearestNeighbor(
        const Point<DataType, DimensionType> &query,
        const Samples<DataType> &samples,
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

    static NearestNeighbor<DataType, DimensionType> findNearestNeighbor(
        const Samples<DataType> &samples,
        const KDTree<DataType, DimensionType> &kdTree,
        const Point<DataType, DimensionType> &query
    ) {

        concurrent_vector<DimensionType> potentialNeighbors;
        DataToSearchNodes<DataType, DimensionType> dataToSearchNodes (
            kdTree,
            query,
            0,
            &potentialNeighbors
        );
        vector<DataToSearchNodes<DataType, DimensionType>> list {dataToSearchNodes};
        parallel_do( list.begin(), list.end(), ParallelNodeExplorer<DataType, DimensionType>() );

    }
};



#endif //KDTREE_KDTREEEXPLORER_H
