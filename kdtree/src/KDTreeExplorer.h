#ifndef KDTREE_KDTREEEXPLORER_H
#define KDTREE_KDTREEEXPLORER_H

#include "memory"
#include "KDTree.h"
#include <stack>
#include <math.h>

using namespace std;

class query_and_kd_tree_dimensions_dont_match : public exception {};

template <typename DataType, typename DimensionType>
class KDTreeExplorer {
public:

    static NearestNeighbor<DataType, DimensionType> *findNearestNeighbor(
        const Samples<DataType> &samples,
        const KDTree<DataType, DimensionType> &kdTree,
        const Point<DataType, DimensionType> &query
    ) {
        Segment<DataType, DimensionType> segment(samples);

        // Check query and KD-tree dimensions match
        if(segment.getNumDimensions() != query.getCoefficients().size()) {
            throw query_and_kd_tree_dimensions_dont_match();
        }

        NearestNeighbor<DataType, DimensionType> *currentNearestNeighbor =
            new NearestNeighbor<DataType, DimensionType>(0, segment.getPoint(0), numeric_limits<double>::infinity());

        stack<Node<DataType, DimensionType>*> nodesToExplore;
        Node<DataType, DimensionType> * rootNode = kdTree.getNode(0);
        nodesToExplore.push(rootNode);

        while (!nodesToExplore.empty()) {
            Node<DataType, DimensionType> *node = nodesToExplore.top();
            nodesToExplore.pop();

            const DimensionType nodeId = node->getId();

            if (node->getType() == NodeType::LEAF) {
                Point<DataType, DimensionType> point = segment.getPoint(node->getSampleId());
                const DataType distanceFromPoint = Point<DataType, DimensionType>::squaredDistance(point, query);
                if (distanceFromPoint < currentNearestNeighbor->getSquaredDistance()) {
                    currentNearestNeighbor =
                        new NearestNeighbor<DataType, DimensionType>(node->getSampleId(), point, distanceFromPoint);
                }
            } else {
                const DimensionType leftNodeId = kdTree.leftNodeId(nodeId);
                Node<DataType, DimensionType> *leftNodeToExplore = kdTree.getNode(leftNodeId);

                const DimensionType rightNodeId = kdTree.rightNodeId(nodeId);
                Node<DataType, DimensionType> *rightNodeToExplore = kdTree.getNode(rightNodeId);

                pair<DataType, DataType> distances = node->getSplit()->getDistancesFromSplits(query);
                DataType distanceFromLeftNode = distances.first;
                DataType distanceFromRightNode = distances.second;
                DataType currentDistance = currentNearestNeighbor->getSquaredDistance();

                bool insertLeftNode = distanceFromLeftNode <= currentDistance;
                bool insertRightNode = distanceFromRightNode <= currentDistance;

                if (insertLeftNode && insertRightNode) {
                    if (distanceFromLeftNode < distanceFromRightNode) {
                        nodesToExplore.push(rightNodeToExplore);
                        nodesToExplore.push(leftNodeToExplore);
                    } else {
                        nodesToExplore.push(leftNodeToExplore);
                        nodesToExplore.push(rightNodeToExplore);
                    }
                } else if (insertLeftNode) {
                    nodesToExplore.push(leftNodeToExplore);
                } else if (insertRightNode) {
                    nodesToExplore.push(rightNodeToExplore);
                }
            }
        }

        return currentNearestNeighbor;
    };

};

#endif //KDTREE_KDTREEEXPLORER_H
