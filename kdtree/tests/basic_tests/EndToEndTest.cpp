#include <gmock/gmock.h>

#include "NodeBuilder.h"
#include "KDTreeIO.h"
#include "KDTreeExplorer.h"

class EndToEndTest : public testing::Test {
protected:
    virtual void SetUp() {
        char cCurrentPath[FILENAME_MAX];
        getcwd(cCurrentPath, sizeof(cCurrentPath));
        string currentWorkingDir = string(cCurrentPath);
        kdtreeFolder = currentWorkingDir.substr(0, currentWorkingDir.find("kdtree")+7);
    }

    string kdtreeFolder;
};

TEST_F(EndToEndTest, test_dummy_data) {

//    char cCurrentPath[FILENAME_MAX];
//    getcwd(cCurrentPath, sizeof(cCurrentPath));
//    string currentWorkingDir = string(cCurrentPath);
//    string kdtreeFolder = currentWorkingDir.substr(0, currentWorkingDir.find("kdtree")+7);

    Samples<float> samples = KDTreeIO<float, int>::loadSamples(kdtreeFolder + "/tests/basic_tests/data/dummy_data.csv");
    int numSamples = samples.rows();
    vector<int> sampleIdsInSegment (numSamples);
    iota(begin(sampleIdsInSegment), end(sampleIdsInSegment), 0);

    int depth = log2(numSamples);
    int numNodes = pow(2, depth) - 1 + numSamples;
    KDTree<float, int> kdTree(numNodes);

    DataToBuildNodes<float, int> dataForIteration = DataToBuildNodes<float, int>(
        samples,
        sampleIdsInSegment,
        0,
        DimensionSelectorType::CYCLE_THROUGH_AXES,
        SplittingMethod::MEDIAN_OF_MEDIAN1,
        -1,
        &kdTree
    );

    NodeBuilder<float, int>::build(dataForIteration);

    Segment<float, int> segment (samples, sampleIdsInSegment);

    vector<float> query {1.8, 1};

    NearestNeighbor<float, int> *nearestNeighbor = ParallelNodeExplorer<float, int>::findNearestNeighbor(
            samples,
            kdTree,
            query
    );

    vector<float> expectedNN {2, 1.1};
    EXPECT_EQ(expectedNN, nearestNeighbor->getPoint().getCoefficients());

    int i = 10;

}
