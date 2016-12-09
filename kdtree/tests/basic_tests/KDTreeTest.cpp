#include "memory"
#include <gmock/gmock.h>
#include "KDTree.h"

class NodeAndKDTreeTest : public testing::Test {
protected:
    virtual void SetUp() {
        splitPoint = new Point<float, int>(splitPointV);
        leftBounds = new Bounds<float, int>(maxPointV, minPointV);
        rightBounds = new Bounds<float, int>(maxPointV, minPointV);
        dimensionWithSplitInfo = new Split<float, int>(1, *splitPoint, *leftBounds, *rightBounds);
    }

    vector<float> splitPointV {3,4};
    vector<float> maxPointV {5,6};
    vector<float> minPointV {1,2};

    Point<float, int> *splitPoint;
    Bounds<float, int> *leftBounds;
    Bounds<float, int> *rightBounds;
    Split<float, int> *dimensionWithSplitInfo;

};

TEST_F(NodeAndKDTreeTest, test_leaf_node) {
    Node<float, int> node (NodeType::LEAF, 1, 10);
    ASSERT_EQ(NodeType::LEAF, node.getType());
    ASSERT_EQ(10, node.getSampleId());
}

TEST_F(NodeAndKDTreeTest, test_internal_node) {
    Node<float, int> node (NodeType::INTERNAL, 1, *dimensionWithSplitInfo);
    ASSERT_EQ(NodeType::INTERNAL, node.getType());
    ASSERT_EQ(1, node.getSplit()->getSplitDimension());
    ASSERT_EQ(4.0, node.getSplit()->getSplitThreshold());
    EXPECT_EQ(maxPointV, node.getSplit()->getLeftBounds().getMaxPoint().getCoefficients());
    EXPECT_EQ(minPointV, node.getSplit()->getLeftBounds().getMinPoint().getCoefficients());
    EXPECT_EQ(maxPointV, node.getSplit()->getRightBounds().getMaxPoint().getCoefficients());
    EXPECT_EQ(minPointV, node.getSplit()->getRightBounds().getMinPoint().getCoefficients());
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
    ASSERT_EQ(1, kdTree.getNode(0)->getSplit()->getSplitDimension());
    ASSERT_EQ(4.0, kdTree.getNode(0)->getSplit()->getSplitThreshold());
    ASSERT_EQ(-1, kdTree.getNode(0)->getSampleId());

    ASSERT_EQ(NodeType::LEAF, kdTree.getNode(1)->getType());
    ASSERT_EQ(nullptr, kdTree.getNode(1)->getSplit());
    ASSERT_EQ(0, kdTree.getNode(1)->getSampleId());

    ASSERT_EQ(NodeType::LEAF, kdTree.getNode(2)->getType());
    ASSERT_EQ(nullptr, kdTree.getNode(2)->getSplit());
    ASSERT_EQ(1, kdTree.getNode(2)->getSampleId());
}

TEST_F(NodeAndKDTreeTest, test_string_serilization_empty_node) {
    Node<float, int> node1 (NodeType::EMPTY, 1);
    Node<float, int> node2 = Node<float, int>::fromString(node1.toString());
    ASSERT_EQ(NodeType::EMPTY, node2.getType());
    ASSERT_EQ(1, node2.getId());
    EXPECT_FALSE(node2.getSplit());
}

TEST_F(NodeAndKDTreeTest, test_string_serilization_leaf_node) {
    Node<float, int> node1 (NodeType::LEAF, 1, 10);
    Node<float, int> node2 = Node<float, int>::fromString(node1.toString());
    ASSERT_EQ(NodeType::LEAF, node2.getType());
    ASSERT_EQ(1, node2.getId());
    ASSERT_EQ(10, node2.getSampleId());
    EXPECT_FALSE(node2.getSplit());
}

TEST_F(NodeAndKDTreeTest, test_string_serilization_internal_node) {
    Node<float, int> node1 (NodeType::INTERNAL, 1, *dimensionWithSplitInfo);
    Node<float, int> node2 = Node<float, int>::fromString(node1.toString());
    ASSERT_EQ(NodeType::INTERNAL, node2.getType());
    ASSERT_EQ(1, node2.getId());
    ASSERT_EQ(-1, node2.getSampleId());
    ASSERT_EQ(1, node2.getSplit()->getSplitDimension());
    ASSERT_EQ(4.0, node2.getSplit()->getSplitThreshold());
}

//TEST(KDTreeSearchTest, test_search_in_kd_tree) {
//    KDTree<float, int> kdtree(4);
//    vector<float> query {1,2};
//    Point<float, int> nn = KDTree<float, int>::nearestNeighbor(kdtree, query);
//    float distance = Point<float, int>::euclideanDistance(query, nn);
//    EXPECT_NEAR(0, distance, 0.01);
//}
