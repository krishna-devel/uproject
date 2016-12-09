#include <gmock/gmock.h>

#include "KDTreeHandler.h"
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

class KDTreeHandlerTest : public testing::Test {
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

    void runTest(const string &dataSet, const DimensionSelectorType &dimensionSelectorType, const bool parallel) {
        string samplesFileName = kdtreeFolder + "/tests/basic_tests/data/" + dataSet + ".csv";
        string queriesFileName = kdtreeFolder + "/tests/basic_tests/data/" + dataSet + ".queries.csv";
        string expectedNNFileName = kdtreeFolder + "/tests/basic_tests/data/" + dataSet + ".nn.csv";

        Samples<double> samples = KDTreeIO<double, int>::loadSamples(samplesFileName);
//        int numSamples = samples.rows();
//        vector<int> sampleIdsInSegment(numSamples);
//        iota(begin(sampleIdsInSegment), end(sampleIdsInSegment), 0);
//
//        int numNodes = KDTree<float, int>::getNumNodes(numSamples);
//
//        KDTree<double, int> kdTree(numNodes);
//        DataToBuildNodes<double, int> dataForIteration = DataToBuildNodes<double, int>(
//                samples,
//                sampleIdsInSegment,
//                0,
//                dimensionSelectorType,
//                SplittingMethod::MEDIAN_OF_MEDIAN,
//                -1,
//                &kdTree
//        );

        // Test building, writing and loading of trees
        string modelFileName = tmpnam(nullptr);
//        if (parallel) {
//            KDTreeBuilder<double, int>::buildInParallel(dataForIteration);
//        }
//        else {
//            KDTreeBuilder<double, int>::buildUsingRecursion(dataForIteration);
//        }
//        KDTreeIO<double, int>::write(kdTree, modelFileName);

        KDTreeHandler<double, long>::buildKDTree(
                samplesFileName,
                modelFileName,
                dimensionSelectorType,
                SplittingMethod::MEDIAN_OF_MEDIAN,
                parallel
        );


        KDTree<double, int> loadedKDTree = KDTreeIO<double, int>::load(modelFileName);

        // Test NN search on built tree
        Samples<double> queries = KDTreeIO<double, int>::loadSamples(queriesFileName);
        int numQueries = queries.rows();
        vector<int> queryIdsInSegment(numQueries);
        iota(begin(queryIdsInSegment), end(queryIdsInSegment), 0);

        vector<NN *> expectedNNs = loadNN(expectedNNFileName);

        Segment<double, int> samplesSegment(samples);
        Segment<double, int> querySegment(queries);

        for (int queryId : queryIdsInSegment) {
            NearestNeighbor<double, int> *nearestNeighbor = KDTreeExplorer<double, int>::findNearestNeighbor(
                    samples,
                    loadedKDTree,
                    querySegment.getPoint(queryId)
            );

            Point<double, int> expectedNN = samplesSegment.getPoint(expectedNNs[queryId]->getId());
            double expectedDistance = expectedNNs[queryId]->getDistance();
            EXPECT_EQ(expectedNN.getCoefficients(), nearestNeighbor->getPoint().getCoefficients());
            EXPECT_NEAR(expectedDistance, nearestNeighbor->getEuclideanDistance(), 0.00001);
        }
    }

    void runTests(const string &dataSet) {
        runTest(dataSet, DimensionSelectorType::CYCLE_THROUGH_AXES, true);
        runTest(dataSet, DimensionSelectorType::HIGHEST_RANGE_AXIS, true);
        runTest(dataSet, DimensionSelectorType::HIGHEST_RANGE_AXIS, false);
    }

    virtual void SetUp() {
        char cCurrentPath[FILENAME_MAX];
        getcwd(cCurrentPath, sizeof(cCurrentPath));
        string currentWorkingDir = string(cCurrentPath);
        kdtreeFolder = currentWorkingDir.substr(0, currentWorkingDir.find("kdtree")+7);
    }

    string kdtreeFolder;
};

TEST_F(KDTreeHandlerTest, test_dummy_data) { runTests("dummy_data"); }
TEST_F(KDTreeHandlerTest, test_sample_data) { runTests("sample_data"); }
//TEST_F(KDTreeHandlerTest, test_test_25d) { runTests("test_25d"); }
TEST_F(KDTreeHandlerTest, test_test_50K) { runTests("test_50K"); }
