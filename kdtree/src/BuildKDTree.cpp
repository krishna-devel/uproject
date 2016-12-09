#include <iostream>
#include <boost/program_options.hpp>
#include <SplitDimensionSelector.h>
#include <SplitPointSelector.h>

using namespace std;
using namespace boost;
namespace po = boost::program_options;

DimensionSelectorType getDimensionSelectorType(const string &argName, const string &axisSelector) {
    DimensionSelectorType dimensionSelectorType;
    if (axisSelector == "CYCLE_THROUGH_AXES"){
        dimensionSelectorType = DimensionSelectorType::CYCLE_THROUGH_AXES;
    } else if (axisSelector == "HIGHEST_RANGE_AXIS"){
        dimensionSelectorType = DimensionSelectorType::HIGHEST_RANGE_AXIS;
    } else {
        throw po::validation_error(po::validation_error::invalid_option_value, argName, axisSelector);
    }
    return dimensionSelectorType;
}

SplittingMethod getSplittingMethod(const string &argName, const string &splitPositionAlgorithm) {
    SplittingMethod splittingMethod;
    if (splitPositionAlgorithm == "MEDIAN"){
        splittingMethod = SplittingMethod::MEDIAN;
    } else if (splitPositionAlgorithm == "MEDIAN_OF_MEDIAN"){
        splittingMethod = SplittingMethod::MEDIAN_OF_MEDIAN;
    } else {
        throw po::validation_error(po::validation_error::invalid_option_value, argName, splitPositionAlgorithm);
    }
    return splittingMethod;
}

int main(int argc, char* argv[]) {

    string inputSampleDataset;
    string outputModelFilename;
    string splitAxisAlgorithm;
    string splitPositionAlgorithm;
    bool parallelExecution;
    bool printExecutionTime;

    try {
        po::options_description desc("Options");
        desc.add_options()
                ("input,i", po::value<string>(&inputSampleDataset)->required(), "Dataset used to build kd-tree (csv)")
                ("output,o", po::value<string>(&outputModelFilename)->required(), "Model to store built kd-tree")
                ("split_axis_algorithm,a", po::value<string>(&splitAxisAlgorithm)->default_value("HIGHEST_RANGE_AXIS"),
                   "Algorithm used to select axis to split on. Select one of: [ CYCLE_THROUGH_AXES, HIGHEST_RANGE_AXIS ]")
                ("split_position_algorithm,s", po::value<string>(&splitPositionAlgorithm)->default_value("MEDIAN_OF_MEDIAN"),
                   "Algorithm used to select split position. Select one of: [ MEDIAN, MEDIAN_OF_MEDIAN ]")
                ("parallel,p", po::value<bool>(&parallelExecution)->default_value(true), "Build model in parallel")
                ("print_execution_time,t", po::value<bool>(&printExecutionTime)->default_value(true), "Prints execution time in seconds")
                ("help,h", "Produce help message");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (argc <= 1 || vm.count("help")) {
            cout << desc << endl;
            return 1;
        }

        po::notify(vm);

        DimensionSelectorType dimensionSelectorType = getDimensionSelectorType("split_axis_algorithm", splitAxisAlgorithm);
        SplittingMethod splittingMethod = getSplittingMethod("split_position_algorithm", splitPositionAlgorithm);

        cout << "Reading samples from: " << inputSampleDataset << endl;
        cout << "Algorithm used to select axis to split: " << dimensionSelectorType << endl;
        cout << "Algorithm used to select split position: " << splittingMethod << endl;
        cout << "Building model in parallel: " << parallelExecution << endl;

        clock_t start;
        double duration;
        start = clock();

        cout << "Building kd-tree..." << endl;

        duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        for (int i = 0; i <=1000000; i++){

        }

        cout << "Writing model to: " << outputModelFilename << endl;

        cout<<"Model was built in "<< duration << " seconds." << endl;

    } catch(std::exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return false;
    } catch(...) {
        cerr << "Unknown error!" << "\n";
        return false;
    }
}