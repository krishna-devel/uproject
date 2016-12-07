#include <gmock/gmock.h>

#include "NodeBuilder.h"
#include "KDTreeIO.h"
#include "KDTreeExplorer.h"

class NN {
private:
    const int id;
    const double distance;
public:
    NN(const int id, const double distance) : id(id), distance(distance) {}
    const int getId() const { return id; }
    const double getDistance() const { return distance; }
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

    vector<string> dataSets {"dummy_data", "sample_data"};

    for(string dataSet : dataSets) {

        string samplesFileName = kdtreeFolder + "/tests/basic_tests/data/" + dataSet + ".csv";
        string queriesFileName = kdtreeFolder + "/tests/basic_tests/data/" + dataSet + ".queries.csv";
        string expectedNNFileName = kdtreeFolder + "/tests/basic_tests/data/" + dataSet + ".nn.csv";

        Samples<double> samples = KDTreeIO<double, int>::loadSamples(samplesFileName);
        int numSamples = samples.rows();
        vector<int> sampleIdsInSegment(numSamples);
        iota(begin(sampleIdsInSegment), end(sampleIdsInSegment), 0);

        int numNodes = KDTree<float, int>::getNumNodes(numSamples);

        KDTree<double, int> kdTree(numNodes);
        DataToBuildNodes<double, int> dataForIteration = DataToBuildNodes<double, int>(
                samples,
                sampleIdsInSegment,
                0,
                DimensionSelectorType::CYCLE_THROUGH_AXES,
                SplittingMethod::MEDIAN_OF_MEDIAN1,
                -1,
                &kdTree
        );

        string modelFileName = tmpnam(nullptr);
        NodeBuilder<double, int>::build(dataForIteration);
        KDTreeIO<double, int>::write(kdTree, modelFileName);
        KDTree<double, int> loadedKDTree = KDTreeIO<double, int>::load(modelFileName);

        Samples<double> queries = KDTreeIO<double, int>::loadSamples(queriesFileName);
        int numQueries = queries.rows();
        vector<int> queryIdsInSegment(numQueries);
        iota(begin(queryIdsInSegment), end(queryIdsInSegment), 0);

        vector<NN *> expectedNNs = loadNN(expectedNNFileName);

        Segment<double, int> samplesSegment(samples);
        Segment<double, int> querySegment(queries);

        for (int queryId : queryIdsInSegment) {
            NearestNeighbor<double, int> *nearestNeighbor = ParallelNodeExplorer<double, int>::findNearestNeighbor(
                    samples,
                    loadedKDTree,
                    querySegment.getPoint(queryId)
            );

            Point<double, int> expectedNN = samplesSegment.getPoint(expectedNNs[queryId]->getId());
            double expectedDistance = expectedNNs[queryId]->getDistance();
            EXPECT_EQ(expectedNN.getCoefficients(), nearestNeighbor->getPoint().getCoefficients());
            EXPECT_NEAR(expectedDistance, nearestNeighbor->getEuclideanDistance(), 0.00001);
        }

////    vector<double> query {3.812286312667677590e-01,2.249309626074655899e-01,5.286729037289117361e-01};
////    vector<double> query {4.021451295452648234e-01,5.366133364034287867e-01,5.365332178253696682e-01};
//    vector<double> query {1.8,1};
//    NearestNeighbor<double, int> *nearestNeighbor = ParallelNodeExplorer<double, int>::findNearestNeighbor(
//        samples,
//        loadedKDTree,
//        query
//    );
//
//    EXPECT_EQ(query, nearestNeighbor->getPoint().getCoefficients());
//    EXPECT_NEAR(0, nearestNeighbor->getEuclideanDistance(), 0.00001);
    }

}
