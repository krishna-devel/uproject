#include "memory"
#include <gmock/gmock.h>
#include "NodeBuilder.h"
#include <unistd.h>
#include <KDTreeIO.h>
#include <math.h>
#include <algorithm>

class NodeBuilderTest : public testing::Test {
protected:
    virtual void SetUp() {
        samples <<
                1.0, 2.0, 3.0,
                4.0, 5.0, 6.0,
                7.0, 8.0, 9.0;
        segment = new Segment<float, int>(samples);
        kdTree = new KDTree<float, int>(5);
    }

    Samples<float> samples {3, 3};
    Segment<float, int> *segment;
    KDTree<float, int> *kdTree;
};

TEST_F(NodeBuilderTest, test_basic_for_cycle_through_axes) {

    for (int iteration = 0; iteration < 2; iteration++) {
        vector<int> ids {0, 1, 2};
//        NodeBuilderParams<float, int> params (
//            0,
//            DimensionSelectorType::CYCLE_THROUGH_AXES,
//            SplittingMethod::MEDIAN_OF_MEDIAN1,
//            -1
//        );
        DataForIteration<float, int> dataForIteration = DataForIteration<float, int>(
                samples,
                ids,
                0,
                DimensionSelectorType::CYCLE_THROUGH_AXES,
                SplittingMethod::MEDIAN_OF_MEDIAN1,
                -1,
                kdTree
        );
        if (iteration == 0) {
            NodeBuilder<float, int>::buildNonRecursive(dataForIteration);
        } else {
            NodeBuilder<float, int>::build(dataForIteration);
        }

        NodeBuilder<float, int>::build(dataForIteration);
        ASSERT_EQ(NodeType::INTERNAL, kdTree->getNode(0)->getType());
        ASSERT_EQ(0, kdTree->getNode(0)->getSplit()->getSplitDimension());
        ASSERT_EQ(4.0, kdTree->getNode(0)->getSplit()->getSplitThreshold());
        ASSERT_EQ(-1, kdTree->getNode(0)->getSampleId());

        ASSERT_EQ(NodeType::INTERNAL, kdTree->getNode(1)->getType());
        ASSERT_EQ(1, kdTree->getNode(1)->getSplit()->getSplitDimension());
        ASSERT_EQ(3.5, kdTree->getNode(1)->getSplit()->getSplitThreshold());
        ASSERT_EQ(-1, kdTree->getNode(1)->getSampleId());

        ASSERT_EQ(NodeType::LEAF, kdTree->getNode(2)->getType());
        ASSERT_EQ(nullptr, kdTree->getNode(2)->getSplit());
        ASSERT_EQ(2, kdTree->getNode(2)->getSampleId());

        ASSERT_EQ(NodeType::LEAF, kdTree->getNode(3)->getType());
        ASSERT_EQ(nullptr, kdTree->getNode(3)->getSplit());
        ASSERT_EQ(0, kdTree->getNode(3)->getSampleId());

        ASSERT_EQ(NodeType::LEAF, kdTree->getNode(4)->getType());
        ASSERT_EQ(nullptr, kdTree->getNode(4)->getSplit());
        ASSERT_EQ(1, kdTree->getNode(4)->getSampleId());

    }

}

TEST_F(NodeBuilderTest, test_basic_for_highest_range_axis) {

    for (int iteration = 0; iteration < 2; iteration++) {
        vector<int> ids{0, 1, 2};
//        NodeBuilderParams<float, int> params(
//            0,
//            DimensionSelectorType::HIGHEST_RANGE_AXIS,
//            SplittingMethod::MEDIAN_OF_MEDIAN1,
//            -1
//        );
//        if (iteration == 0) {
//            NodeBuilder<float, int>::buildNonRecursive(DataForIteration<float, int>(samples, ids, params, kdTree));
//        } else {
//            NodeBuilder<float, int>::build(DataForIteration<float, int>(samples, ids, params, kdTree));
//        }

        DataForIteration<float, int> dataForIteration = DataForIteration<float, int>(
                samples,
                ids,
                0,
                DimensionSelectorType::HIGHEST_RANGE_AXIS,
                SplittingMethod::MEDIAN_OF_MEDIAN1,
                -1,
                kdTree
        );
        if (iteration == 0) {
            NodeBuilder<float, int>::buildNonRecursive(dataForIteration);
        } else {
            NodeBuilder<float, int>::build(dataForIteration);
        }

        ASSERT_EQ(NodeType::INTERNAL, kdTree->getNode(0)->getType());
        ASSERT_EQ(0, kdTree->getNode(0)->getSplit()->getSplitDimension());
        ASSERT_EQ(4.0, kdTree->getNode(0)->getSplit()->getSplitThreshold());
        ASSERT_EQ(-1, kdTree->getNode(0)->getSampleId());

        ASSERT_EQ(NodeType::INTERNAL, kdTree->getNode(1)->getType());
        ASSERT_EQ(0, kdTree->getNode(1)->getSplit()->getSplitDimension());
        ASSERT_EQ(2.5, kdTree->getNode(1)->getSplit()->getSplitThreshold());
        ASSERT_EQ(-1, kdTree->getNode(1)->getSampleId());

        ASSERT_EQ(NodeType::LEAF, kdTree->getNode(2)->getType());
        ASSERT_EQ(nullptr, kdTree->getNode(2)->getSplit());
        ASSERT_EQ(2, kdTree->getNode(2)->getSampleId());

        ASSERT_EQ(NodeType::LEAF, kdTree->getNode(3)->getType());
        ASSERT_EQ(nullptr, kdTree->getNode(3)->getSplit());
        ASSERT_EQ(0, kdTree->getNode(3)->getSampleId());

        ASSERT_EQ(NodeType::LEAF, kdTree->getNode(4)->getType());
        ASSERT_EQ(nullptr, kdTree->getNode(4)->getSplit());
        ASSERT_EQ(1, kdTree->getNode(4)->getSampleId());

    }
}

TEST_F(NodeBuilderTest, test_basic_tree_creation_for_dummy_data) {

    char cCurrentPath[FILENAME_MAX];
    getcwd(cCurrentPath, sizeof(cCurrentPath));
    string currentWorkingDir = string(cCurrentPath);
    string kdtreeFolder = currentWorkingDir.substr(0, currentWorkingDir.find("kdtree")+7);

    Samples<float> samples = KDTreeIO<float, int>::loadSamples(kdtreeFolder + "/tests/basic_tests/data/dummy_data.csv");
    int numSamples = samples.rows();
    vector<int >sampleIdsInSegment (numSamples);
    iota(begin(sampleIdsInSegment), end(sampleIdsInSegment), 0);

//    NodeBuilderParams<float, int> params(
//        0,
//        DimensionSelectorType::CYCLE_THROUGH_AXES,
//        SplittingMethod::MEDIAN_OF_MEDIAN1,
//        -1
//    );

    int depth = log2(numSamples);
    int numNodes = pow(2, depth) - 1 + numSamples;
    KDTree<float, int> kdTree(numNodes);

    DataForIteration<float, int> dataForIteration = DataForIteration<float, int>(
            samples,
            sampleIdsInSegment,
            0,
            DimensionSelectorType::CYCLE_THROUGH_AXES,
            SplittingMethod::MEDIAN_OF_MEDIAN1,
            -1,
            &kdTree
    );

    NodeBuilder<float, int>::build(dataForIteration);
    KDTreeIO<float, int>::write(kdTree, "debug_tree");
    int i = 10;

}