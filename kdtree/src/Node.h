//
// Created by Krishna Kamath on 11/22/16.
//

#ifndef KDTREE_NODE_H
#define KDTREE_NODE_H

using namespace std;

template <typename  DataType>
class SplitInfo {
public:
    SplitInfo(DataType threshold) : splitThreshold(threshold) {}
    DataType getThreshold() const { return splitThreshold; }
private:
    DataType splitThreshold;
};

//template <typename  T>
//class Node {
//private:
//    SplitInfo<T> splitInfo;
//    unique_ptr<Node<T>> leftChild;
//    unique_ptr<Node<T>> rigthChild;
//};

#endif //KDTREE_NODE_H
