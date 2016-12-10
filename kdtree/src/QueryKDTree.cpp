#include <iostream>
#include <boost/program_options.hpp>
#include "KDTreeHandler.h"

using namespace std;
using namespace boost;
namespace po = boost::program_options;

int main(int argc, char* argv[]) {

    string inputSampleDataset;
    string modelFilename;
    string queriesFileName;
    string nnOuputDataset;

    try {
        po::options_description desc("Options");
        desc.add_options()
                ("input,i", po::value<string>(&inputSampleDataset)->required(), "Dataset used to build kd-tree (csv)")
                ("model,m", po::value<string>(&modelFilename)->required(), "Model with built kd-tree")
                ("queries,q", po::value<string>(&queriesFileName)->required(), "File containing queries to search")
                ("nn_output,n", po::value<string>(&nnOuputDataset)->required(), "Nearest neighbor output")
                ("help,h", "Produce help message");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (argc <= 1 || vm.count("help")) {
            cout << desc << endl;
            return 1;
        }

        po::notify(vm);

        cout << "Using samples from: " << inputSampleDataset << endl;
        cout << "Loading model file from: " << modelFilename << endl;
        cout << "Loading queries from: " << queriesFileName << endl;

        clock_t start;
        double duration;
        start = clock();

        cout << "Building kd-tree..." << endl;

        KDTreeHandler<double, long>::queryKDTree(
                inputSampleDataset,
                modelFilename,
                queriesFileName,
                nnOuputDataset
        );

        cout << "Storing nearest neighbors in: " << nnOuputDataset << endl;

        duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        cout<<"Searched queries in "<< duration << " seconds." << endl;

    } catch(std::exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return false;
    } catch(...) {
        cerr << "Unknown error!" << "\n";
        return false;
    }
}