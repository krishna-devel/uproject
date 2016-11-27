#include "memory"
#include <gmock/gmock.h>
#include "NodeBuilder.h"

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
    unique_ptr<NodeBuilderParams<float, int>> params = unique_ptr<NodeBuilderParams<float, int>> (
        new NodeBuilderParams<float, int>(
            0,
            DimensionSelectorType::CYCLE_THROUGH_AXES,
            DimensionSplittingMethod::MEDIAN_OF_MEDIAN,
            -1
        )
    );
    NodeBuilder<float, int>::build(params, *segment, kdTree);
    ASSERT_EQ(NodeType::INTERNAL, kdTree->getNode(0)->getType());
    ASSERT_EQ(0, kdTree->getNode(0)->getDimensionWithSplitInfo()->getSplitDimension());
    ASSERT_EQ(4.0, kdTree->getNode(0)->getDimensionWithSplitInfo()->getSplitInfo().getThreshold());
    ASSERT_EQ(-1, kdTree->getNode(0)->getSampleId());

    ASSERT_EQ(NodeType::INTERNAL, kdTree->getNode(1)->getType());
    ASSERT_EQ(1, kdTree->getNode(1)->getDimensionWithSplitInfo()->getSplitDimension());
    ASSERT_EQ(3.5, kdTree->getNode(1)->getDimensionWithSplitInfo()->getSplitInfo().getThreshold());
    ASSERT_EQ(-1, kdTree->getNode(1)->getSampleId());

    ASSERT_EQ(NodeType::LEAF, kdTree->getNode(2)->getType());
    ASSERT_EQ(nullptr, kdTree->getNode(2)->getDimensionWithSplitInfo());
    ASSERT_EQ(2, kdTree->getNode(2)->getSampleId());

    ASSERT_EQ(NodeType::LEAF, kdTree->getNode(3)->getType());
    ASSERT_EQ(nullptr, kdTree->getNode(3)->getDimensionWithSplitInfo());
    ASSERT_EQ(0, kdTree->getNode(3)->getSampleId());

    ASSERT_EQ(NodeType::LEAF, kdTree->getNode(4)->getType());
    ASSERT_EQ(nullptr, kdTree->getNode(4)->getDimensionWithSplitInfo());
    ASSERT_EQ(1, kdTree->getNode(4)->getSampleId());
}

TEST_F(NodeBuilderTest, test_basic_for_highest_range_axis) {
    unique_ptr<NodeBuilderParams<float, int>> params = unique_ptr<NodeBuilderParams<float, int>> (
        new NodeBuilderParams<float, int>(
            0,
            DimensionSelectorType::HIGHEST_RANGE_AXIS,
            DimensionSplittingMethod::MEDIAN_OF_MEDIAN,
            -1
        )
    );
    NodeBuilder<float, int>::build(params, *segment, kdTree);
    ASSERT_EQ(NodeType::INTERNAL, kdTree->getNode(0)->getType());
    ASSERT_EQ(0, kdTree->getNode(0)->getDimensionWithSplitInfo()->getSplitDimension());
    ASSERT_EQ(4.0, kdTree->getNode(0)->getDimensionWithSplitInfo()->getSplitInfo().getThreshold());
    ASSERT_EQ(-1, kdTree->getNode(0)->getSampleId());

    ASSERT_EQ(NodeType::INTERNAL, kdTree->getNode(1)->getType());
    ASSERT_EQ(0, kdTree->getNode(1)->getDimensionWithSplitInfo()->getSplitDimension());
    ASSERT_EQ(2.5, kdTree->getNode(1)->getDimensionWithSplitInfo()->getSplitInfo().getThreshold());
    ASSERT_EQ(-1, kdTree->getNode(1)->getSampleId());

    ASSERT_EQ(NodeType::LEAF, kdTree->getNode(2)->getType());
    ASSERT_EQ(nullptr, kdTree->getNode(2)->getDimensionWithSplitInfo());
    ASSERT_EQ(2, kdTree->getNode(2)->getSampleId());

    ASSERT_EQ(NodeType::LEAF, kdTree->getNode(3)->getType());
    ASSERT_EQ(nullptr, kdTree->getNode(3)->getDimensionWithSplitInfo());
    ASSERT_EQ(0, kdTree->getNode(3)->getSampleId());

    ASSERT_EQ(NodeType::LEAF, kdTree->getNode(4)->getType());
    ASSERT_EQ(nullptr, kdTree->getNode(4)->getDimensionWithSplitInfo());
    ASSERT_EQ(1, kdTree->getNode(4)->getSampleId());
}

