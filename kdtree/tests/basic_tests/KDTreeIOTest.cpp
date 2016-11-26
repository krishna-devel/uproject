#include "memory"
#include <gmock/gmock.h>
#include "KDTreeIO.h"

class KDTestIOTest : public testing::Test {
protected:
    virtual void SetUp() {
        splitInfo = new SplitInfo<float>(2.0);
        dimensionWithSplitInfo = new DimensionWithSplitInfo<float, int>(10, *splitInfo);
        kdTree = new KDTree<float, int>(3);
        kdTree->insertInternalNode(0, *dimensionWithSplitInfo);
        kdTree->insertLeafNode(1, 0);
        kdTree->insertLeafNode(2, 1);
    }
    SplitInfo<float>* splitInfo;
    DimensionWithSplitInfo<float, int>* dimensionWithSplitInfo;
    KDTree<float, int>* kdTree;

};
TEST_F(KDTestIOTest, test_basics) {
    KDTreeIO<float, int>::write(*kdTree, "kdTree_from_test", true);
    KDTree<float, int> loadedKDTree = KDTreeIO<float, int>::load("kdTree_from_test");

    ASSERT_EQ(3, loadedKDTree.getNumNodes());

    ASSERT_EQ(NodeType::INTERNAL, loadedKDTree.getNode(0)->getType());
    ASSERT_EQ(NodeType::LEAF, loadedKDTree.getNode(1)->getType());
    ASSERT_EQ(NodeType::LEAF, loadedKDTree.getNode(2)->getType());

    ASSERT_EQ(-1, loadedKDTree.getNode(0)->getSampleId());
    ASSERT_EQ(0, loadedKDTree.getNode(1)->getSampleId());
    ASSERT_EQ(1, loadedKDTree.getNode(2)->getSampleId());

    ASSERT_EQ(10, loadedKDTree.getNode(0)->getDimensionWithSplitInfo()->getSplitDimension());
    ASSERT_EQ(2.0, loadedKDTree.getNode(0)->getDimensionWithSplitInfo()->getSplitInfo().getThreshold());
    EXPECT_FALSE(loadedKDTree.getNode(1)->getDimensionWithSplitInfo());
    EXPECT_FALSE(loadedKDTree.getNode(2)->getDimensionWithSplitInfo());

}