#include <gmock/gmock.h>

#include "NodeBuilder.h"
#include "KDTreeIO.h"
#include "KDTreeExplorer.h"

class NN {
private:
    const int id;
    const float distance;
public:
    NN(const int id, const float distance) : id(id), distance(distance) {}
    const int getId() const { return id; }
    const float getDistance() const { return distance; }
};

class EndToEndTest : public testing::Test {
protected:

    vector<NN *> loadNN(const string &fileName) {
        vector<NN *> expectedNearestNeighbors;

        string line;
        ifstream inputFile (fileName);
        if (inputFile.is_open()) {
            while ( getline (inputFile,line) ) {
                vector<string> tokens = Util::split(line, ",");
                expectedNearestNeighbors.push_back(new NN(stoi(tokens[0]), stof(tokens[1])));
            }
            inputFile.close();
        }
        else {
            cout << "Unable to open file: " + fileName;
        }

        return expectedNearestNeighbors;
    }

    virtual void SetUp() {
        char cCurrentPath[FILENAME_MAX];
        getcwd(cCurrentPath, sizeof(cCurrentPath));
        string currentWorkingDir = string(cCurrentPath);
        kdtreeFolder = currentWorkingDir.substr(0, currentWorkingDir.find("kdtree")+7);
    }

    string kdtreeFolder;
};

TEST_F(EndToEndTest, test_dummy_data) {

    string dataSet = "dummy_data";
    string samplesFileName = kdtreeFolder + "/tests/basic_tests/data/" + dataSet + ".csv";
    string queriesFileName = kdtreeFolder + "/tests/basic_tests/data/" + dataSet + ".queries.csv";
    string expectedNNFileName = kdtreeFolder + "/tests/basic_tests/data/" + dataSet + ".nn.csv";

    Samples<float> samples = KDTreeIO<float, int>::loadSamples(samplesFileName);
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

//    vector<float> query {1.8, 1};
//    NearestNeighbor<float, int> *nearestNeighbor = ParallelNodeExplorer<float, int>::findNearestNeighbor(
//        samples,
//        kdTree,
//        query
//    );

    Samples<float> queries = KDTreeIO<float, int>::loadSamples(queriesFileName);
    int numQueries = queries.rows();
    vector<int> queryIdsInSegment (numQueries);
    iota(begin(queryIdsInSegment), end(queryIdsInSegment), 0);

    vector<NN *> expectedNNs = loadNN(expectedNNFileName);

    Segment<float, int> samplesSegment(samples);
    Segment<float, int> querySegment(queries);

//    for (int queryId : queryIdsInSegment) {
//        NearestNeighbor<float, int> *nearestNeighbor = ParallelNodeExplorer<float, int>::findNearestNeighbor(
//            samples,
//            kdTree,
//            querySegment.getPoint(queryId)
//        );
//
//        Point<float, int> expectedNN = samplesSegment.getPoint(expectedNNs[queryId]->getId());
//        float expectedDistance = expectedNNs[queryId]->getDistance();
//        EXPECT_EQ(expectedNN.getCoefficients(), nearestNeighbor->getPoint().getCoefficients());
//        EXPECT_NEAR(expectedDistance, nearestNeighbor->getEuclideanDistance(), 0.001);
//    }

    vector<float> query {1.5, 2.5};
    NearestNeighbor<float, int> *nearestNeighbor = ParallelNodeExplorer<float, int>::findNearestNeighbor(
        samples,
        kdTree,
        query
    );

    EXPECT_EQ(query, nearestNeighbor->getPoint().getCoefficients());
    EXPECT_NEAR(0, nearestNeighbor->getEuclideanDistance(), 0.001);

//    vector<float> expectedNN {2, 1.1};
//    EXPECT_EQ(expectedNN, nearestNeighbor->getPoint().getCoefficients());

//    vector<NN *> expectedNNs = loadNN(expectedNNFileName);



    int i = 10;

}
