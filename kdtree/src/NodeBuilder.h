#ifndef KDTREE_NODEBUILDER_H
#define KDTREE_NODEBUILDER_H

#include "Node.h"
#include "Segment.h"

template <typename NodeValueType, typename DataType, typename DimensionType>
class NodeBuilder {
public:
    static void build(const Segment<DataType, DimensionType> &segment, unique_ptr<Node<NodeValueType>> *node);
};

template <typename NodeValueType, typename DataType, typename DimensionType>
void NodeBuilder<NodeValueType, DataType, DimensionType>::build(
        const Segment<DataType, DimensionType> &segment,
        unique_ptr<Node<NodeValueType>> *node
) {


}

#endif //KDTREE_NODEBUILDER_H
