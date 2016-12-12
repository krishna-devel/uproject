#include <gmock/gmock.h>
#include "KDTreeExplorer.h"

TEST(KDTreeExplorerTest, test_query_and_kd_tree_dimensions_dont_match) {

    ASSERT_THROW(
        ({
            Samples<float> samples {2, 2};
            samples << 1,2,3,4;
            KDTree<float, int> kdTree (10);
            vector<float> queryWithWrongDimension {1,3,4};
            KDTreeExplorer<float, int>::findNearestNeighbor(samples, kdTree, queryWithWrongDimension);
        }),
        query_and_kd_tree_dimensions_dont_match
    );

}
