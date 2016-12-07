#include "memory"
#include <gmock/gmock.h>
#include "KDTreeIO.h"
#include "string"

#include <unistd.h>

using namespace std;

class KDTestIOTest : public testing::Test {
protected:
    virtual void SetUp() {
        kdTree = new KDTree<float, int>(7);
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

    ASSERT_EQ(7, loadedKDTree.getNumNodes());

    ASSERT_EQ(NodeType::INTERNAL, loadedKDTree.getNode(0)->getType());
    ASSERT_EQ(NodeType::LEAF, loadedKDTree.getNode(1)->getType());
    ASSERT_EQ(NodeType::LEAF, loadedKDTree.getNode(2)->getType());

    ASSERT_EQ(-1, loadedKDTree.getNode(0)->getSampleId());
    ASSERT_EQ(0, loadedKDTree.getNode(1)->getSampleId());
    ASSERT_EQ(1, loadedKDTree.getNode(2)->getSampleId());

    ASSERT_EQ(1, loadedKDTree.getNode(0)->getSplit()->getSplitDimension());
    ASSERT_EQ(4.0, loadedKDTree.getNode(0)->getSplit()->getSplitThreshold());
    EXPECT_FALSE(loadedKDTree.getNode(1)->getSplit());
    EXPECT_FALSE(loadedKDTree.getNode(2)->getSplit());

}

TEST_F(KDTestIOTest, test_loading_samples) {
    char cCurrentPath[FILENAME_MAX];
    getcwd(cCurrentPath, sizeof(cCurrentPath));
    string currentWorkingDir = string(cCurrentPath);
    string kdtreeFolder = currentWorkingDir.substr(0, currentWorkingDir.find("kdtree")+7);

    Samples<float> samples1 = KDTreeIO<float, int>::loadSamples(kdtreeFolder + "/tests/basic_tests/data/sample_data.csv");
    ASSERT_EQ(1000, samples1.rows());
    ASSERT_EQ(3, samples1.cols());

    Samples<float> samples2 = KDTreeIO<float, int>::loadSamples(kdtreeFolder + "/tests/basic_tests/data/dummy_data.csv");
    ASSERT_EQ(8, samples2.rows());
    ASSERT_EQ(2, samples2.cols());
}
