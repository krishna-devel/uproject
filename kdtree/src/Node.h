#ifndef KDTREE_NODE_H
#define KDTREE_NODE_H

#include "memory"

using namespace std;

template <typename T>
class Node {
private:
    T value;
    unique_ptr<Node<T>> rightNode;
    unique_ptr<Node<T>> leftNode;
public:
    Node(T value) : value(value) {}
    T getValue() const { return value; }
    unique_ptr<Node<T>> &getLeftNode() { return leftNode; }
    unique_ptr<Node<T>> &getRightNode() { return rightNode; }
};

#endif //KDTREE_NODE_H
