#include "memory"
#include <gmock/gmock.h>
#include "KDTreeIO.h"

class KDTestIOTest : public testing::Test {
protected:
    virtual void SetUp() {
        kdTree = new KDTree<float, int>(3);
        splitPoint = new Point<float, int>(splitPointV);
        leftBounds = new Bounds<float, int>(maxPointV, minPointV);
        rightBounds = new Bounds<float, int>(maxPointV, minPointV);
        dimensionWithSplitInfo = new Split<float, int>(1, *splitPoint, *leftBounds, *rightBounds);
        kdTree->insertInternalNode(0, *dimensionWithSplitInfo);
        kdTree->insertLeafNode(1, 0);
        kdTree->insertLeafNode(2, 1);
    }
    vector<float> splitPointV {3,4};
    vector<float> maxPointV {5,6};
    vector<float> minPointV {1,2};

    Point<float, int> *splitPoint;
    Bounds<float, int> *leftBounds;
    Bounds<float, int> *rightBounds;
    Split<float, int> *dimensionWithSplitInfo;

    KDTree<float, int>* kdTree;

};

TEST_F(KDTestIOTest, test_basics) {

    string fileName = tmpnam(nullptr);

    KDTreeIO<float, int>::write(*kdTree, fileName);
    KDTree<float, int> loadedKDTree = KDTreeIO<float, int>::load(fileName);

    ASSERT_EQ(3, loadedKDTree.getNumNodes());

    ASSERT_EQ(NodeType::INTERNAL, loadedKDTree.getNode(0)->getType());
    ASSERT_EQ(NodeType::LEAF, loadedKDTree.getNode(1)->getType());
    ASSERT_EQ(NodeType::LEAF, loadedKDTree.getNode(2)->getType());

    ASSERT_EQ(-1, loadedKDTree.getNode(0)->getSampleId());
    ASSERT_EQ(0, loadedKDTree.getNode(1)->getSampleId());
    ASSERT_EQ(1, loadedKDTree.getNode(2)->getSampleId());

    ASSERT_EQ(1, loadedKDTree.getNode(0)->getDimensionWithSplitInfo()->getSplitDimension());
    ASSERT_EQ(4.0, loadedKDTree.getNode(0)->getDimensionWithSplitInfo()->getSplitThreshold());
    EXPECT_FALSE(loadedKDTree.getNode(1)->getDimensionWithSplitInfo());
    EXPECT_FALSE(loadedKDTree.getNode(2)->getDimensionWithSplitInfo());

}