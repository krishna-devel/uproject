#include "memory"
#include <gmock/gmock.h>
#include "KDTree.h"

class NodeAndKDTreeTest : public testing::Test {
protected:
    virtual void SetUp() {
        splitInfo = new SplitInfo<float>(2.0);
        dimensionWithSplitInfo = new DimensionWithSplitInfo<float, int>(10, *splitInfo);
    }
    SplitInfo<float>* splitInfo;
    DimensionWithSplitInfo<float, int>* dimensionWithSplitInfo;

};

TEST_F(NodeAndKDTreeTest, test_leaf_node) {
    Node<float, int> node (NodeType::LEAF, 10);
    ASSERT_EQ(NodeType::LEAF, node.getType());
    ASSERT_EQ(10, node.getSampleId());
}

TEST_F(NodeAndKDTreeTest, test_internal_node) {
    Node<float, int> node (NodeType::INTERNAL, *dimensionWithSplitInfo);
    ASSERT_EQ(NodeType::INTERNAL, node.getType());
    ASSERT_EQ(10, node.getDimensionWithSplitInfo()->getSplitDimension());
    ASSERT_EQ(2.0, node.getDimensionWithSplitInfo()->getSplitInfo().getThreshold());
}

TEST_F(NodeAndKDTreeTest, test_getting_child_nodes) {
    KDTree<float, int> kdTree (5);

    ASSERT_EQ(1, kdTree.leftNodeId(0));
    ASSERT_EQ(2, kdTree.rightNodeId(0));
    ASSERT_EQ(3, kdTree.leftNodeId(1));
    ASSERT_EQ(4, kdTree.rightNodeId(1));
    ASSERT_EQ(5, kdTree.leftNodeId(2));
    ASSERT_EQ(6, kdTree.rightNodeId(2));

    ASSERT_EQ(0, kdTree.parentNodeId(0));
    ASSERT_EQ(0, kdTree.parentNodeId(1));
    ASSERT_EQ(0, kdTree.parentNodeId(2));
    ASSERT_EQ(1, kdTree.parentNodeId(3));
    ASSERT_EQ(1, kdTree.parentNodeId(4));
    ASSERT_EQ(2, kdTree.parentNodeId(5));
    ASSERT_EQ(2, kdTree.parentNodeId(6));
}

TEST_F(NodeAndKDTreeTest, test_inserting_nodes) {
    KDTree<float, int> kdTree (3);
    kdTree.insertInternalNode(0, *dimensionWithSplitInfo);
    kdTree.insertLeafNode(1, 0);
    kdTree.insertLeafNode(2, 1);

    ASSERT_EQ(NodeType::INTERNAL, kdTree.getNode(0)->getType());
    ASSERT_EQ(10, kdTree.getNode(0)->getDimensionWithSplitInfo()->getSplitDimension());
    ASSERT_EQ(2.0, kdTree.getNode(0)->getDimensionWithSplitInfo()->getSplitInfo().getThreshold());
    ASSERT_EQ(-1, kdTree.getNode(0)->getSampleId());

    ASSERT_EQ(NodeType::LEAF, kdTree.getNode(1)->getType());
    ASSERT_EQ(nullptr, kdTree.getNode(1)->getDimensionWithSplitInfo());
    ASSERT_EQ(0, kdTree.getNode(1)->getSampleId());

    ASSERT_EQ(NodeType::LEAF, kdTree.getNode(2)->getType());
    ASSERT_EQ(nullptr, kdTree.getNode(2)->getDimensionWithSplitInfo());
    ASSERT_EQ(1, kdTree.getNode(2)->getSampleId());
}

