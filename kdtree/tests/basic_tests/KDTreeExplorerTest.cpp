//#include <gmock/gmock.h>
//#include "KDTreeExplorer.h"
//
//TEST(ParallelNodeSearcherTest, test_potential_neigbors_is_filled_when_node_is_leaf) {
//
//    KDTree<float, int> kdTree(5);
//    kdTree.insertLeafNode(2, 2);
//    kdTree.insertLeafNode(3, 0);
//    kdTree.insertLeafNode(4, 1);
//
//    vector<float> query {1,2};
//    concurrent_vector<int> potentialNeighbors;
//
//    ParallelNodeExplorer<float, int> parallelNodeExplorer;
//    parallelNodeExplorer.getNodesToSearch(DataToSearchNodes<float, int>(kdTree, query, 2, &potentialNeighbors));
//    parallelNodeExplorer.getNodesToSearch(DataToSearchNodes<float, int>(kdTree, query, 3, &potentialNeighbors));
//    parallelNodeExplorer.getNodesToSearch(DataToSearchNodes<float, int>(kdTree, query, 4, &potentialNeighbors));
//    ASSERT_EQ(2, potentialNeighbors[0]);
//    ASSERT_EQ(0, potentialNeighbors[1]);
//    ASSERT_EQ(1, potentialNeighbors[2]);
//
//}
//
//
//TEST(ParallelNodeSearcherTest, test_potential_neigbors_is_filled_when_node_is_internal) {
//
//    KDTree<float, int> kdTree(5);
//
//    vector<float> maxLeft {-1,2};
//    vector<float> minLeft {-3,0};
//    vector<float> maxRight {3,2};
//    vector<float> minRight {1,0};
//    vector<float> splitPoint {0,0};
//
//    Split<float, int> split(
//        0,
//        splitPoint,
//        Bounds<float, int>(maxLeft, minLeft),
//        Bounds<float, int>(maxRight, minRight)
//    );
//
//    kdTree.insertInternalNode(1, split);
//    kdTree.insertLeafNode(3, 3);
//    kdTree.insertLeafNode(4, 4);
//
//    vector<float> pointInsideLeftBounds {-2,1};
//    vector<float> pointInsideRightBounds {-4,4};
//    vector<float> pointOutsideLeftBoundsOnLeft {2,1};
//    vector<float> pointOutsideRightBoundsOnRight {4,4};
//    vector<float> pointEquidistantFromBounds {0, 4};
//
//    concurrent_vector<int> potentialNeighbors;
//
//    ParallelNodeExplorer<float, int> parallelNodeExplorer;
//    vector<int> nextNeighbors1 = parallelNodeExplorer
//        .getNodesToSearch(DataToSearchNodes<float, int>(kdTree, pointInsideLeftBounds, 1, &potentialNeighbors));
//    ASSERT_EQ(1, nextNeighbors1.size());
//    ASSERT_EQ(3, nextNeighbors1[0]);
//
//    vector<int> nextNeighbors2 = parallelNodeExplorer
//        .getNodesToSearch(DataToSearchNodes<float, int>(kdTree, pointInsideRightBounds, 1, &potentialNeighbors));
//    ASSERT_EQ(1, nextNeighbors2.size());
//    ASSERT_EQ(3, nextNeighbors2[0]);
//
//    vector<int> nextNeighbors3 = parallelNodeExplorer
//        .getNodesToSearch(DataToSearchNodes<float, int>(kdTree, pointOutsideLeftBoundsOnLeft, 1, &potentialNeighbors));
//    ASSERT_EQ(1, nextNeighbors3.size());
//    ASSERT_EQ(4, nextNeighbors3[0]);
//
//    vector<int> nextNeighbors4 = parallelNodeExplorer
//        .getNodesToSearch(DataToSearchNodes<float, int>(kdTree, pointOutsideRightBoundsOnRight, 1, &potentialNeighbors));
//    ASSERT_EQ(1, nextNeighbors4.size());
//    ASSERT_EQ(4, nextNeighbors4[0]);
//
//    vector<int> nextNeighbors5 = parallelNodeExplorer
//        .getNodesToSearch(DataToSearchNodes<float, int>(kdTree, pointEquidistantFromBounds, 1, &potentialNeighbors));
//    ASSERT_EQ(2, nextNeighbors5.size());
//    ASSERT_EQ(4, nextNeighbors5[0]);
//    ASSERT_EQ(3, nextNeighbors5[1]);
//
//}
//
//TEST(ParallelNodeSearcherTest, test_getNearestNeighbor) {
//    vector<float> query {2,1};
//
//    Samples<float> samples {4,2};
//    samples << 100,101,
//            -100,101,
//            1,2,
//            200,200;
//
//    concurrent_vector<int> potentialNeighbors;
//    potentialNeighbors.push_back(0);
//    potentialNeighbors.push_back(2);
//    potentialNeighbors.push_back(3);
//
//    NearestNeighbor<float, int> *nearestNeighbor =
//        ParallelNodeExplorer<float, int>::getNearestNeighbor(samples, query, potentialNeighbors);
//
//    vector<float> expectedNearestNeighbor {1,2};
//    EXPECT_EQ(expectedNearestNeighbor, nearestNeighbor->getPoint()->getCoefficients());
//    EXPECT_NEAR(1.414, nearestNeighbor->getDistance(), 0.001);
//}
