#ifndef KDTREE_KDTREEIO_H
#define KDTREE_KDTREEIO_H

#include "KDTree.h"
#include <string>
#include "memory"
#include <iostream>
#include <fstream>

using namespace std;

template <typename DataType, typename DimensionType>
class KDTreeIO {
public:
    static void write(const KDTree<DataType, DimensionType> &kdTree, const string &modelOutputFilePath, bool compressOutput);
    static KDTree<DataType, DimensionType> load(const string &modelFilePath);
private:
    static vector<string> convertKDTreeToStringVector(const KDTree<DataType, DimensionType> &kdTree);
    static KDTree<DataType, DimensionType> convertStringVectorToKDTree(const vector<string> &stringVector);
    static void writeStringVector(const vector<string> &stringVector, const string &outputFilePath);
    static vector<string> loadStringVector(const string &inputFilePath);
    static void compress(const string &inputFilePath);
    static void unCompress(const string &inputFilePath);
};

template <typename DataType, typename DimensionType>
vector<string> KDTreeIO<DataType, DimensionType>::convertKDTreeToStringVector(
    const KDTree<DataType, DimensionType> &kdTree
) {
    vector<string> nodesVector;
    for(DimensionType i = 0; i < kdTree.getNumNodes(); i++) {
        nodesVector.push_back(kdTree.getNode(i)->toString());
    }
    return nodesVector;
}

template <typename DataType, typename DimensionType>
KDTree<DataType, DimensionType> KDTreeIO<DataType, DimensionType>::convertStringVectorToKDTree(
   const vector<string> &stringVector
) {
    KDTree<DataType, DimensionType> kdTree(stringVector.size());
    for(DimensionType nodeId = 0; nodeId < stringVector.size(); nodeId++) {
        string nodeString = stringVector[nodeId];
        Node<DataType, DimensionType> node = Node<DataType, DimensionType>::fromString(nodeString);
        if (node.getType() == NodeType::INTERNAL) {
            kdTree.insertInternalNode(
                nodeId,
                DimensionWithSplitInfo<DataType, DimensionType>(
                    node.getDimensionWithSplitInfo()->getSplitDimension(),
                    node.getDimensionWithSplitInfo()->getSplitInfo()
                )
            );
        }
        else if (node.getType() == NodeType::LEAF) {
            kdTree.insertLeafNode(nodeId, node.getSampleId());
        }
    }
    return kdTree;
}

template <typename DataType, typename DimensionType>
void KDTreeIO<DataType, DimensionType>::writeStringVector(
    const vector<string> &stringVector,
    const string &outputFilePath
) {
    ofstream outputFile (outputFilePath);
    if (outputFile.is_open()) {
        for(string nodeString: stringVector) outputFile << (nodeString + "\n");
        outputFile.close();
    }
    else cout << "Unable to open file: " + outputFilePath;
}

template <typename DataType, typename DimensionType>
vector<string> KDTreeIO<DataType, DimensionType>::loadStringVector(const string &inputFilePath) {
    vector<string> stringVector;

    string line;
    ifstream inputFile (inputFilePath);
    if (inputFile.is_open()) {
        while ( getline (inputFile,line) ) { stringVector.push_back(line); }
        inputFile.close();
    }
    else cout << "Unable to open file: " + inputFilePath;

    return stringVector;
}

template <typename DataType, typename DimensionType>
void KDTreeIO<DataType, DimensionType>::write(
    const KDTree<DataType, DimensionType> &kdTree,
    const string &modelOutputFilePath,
    bool compressOutput
) {
    writeStringVector(convertKDTreeToStringVector(kdTree), modelOutputFilePath);
    if (compressOutput) {
        compress(modelOutputFilePath);
    }
}

template <typename DataType, typename DimensionType>
void KDTreeIO<DataType, DimensionType>::compress(const string &inputFilePath) {
    string compressCommand = "gzip " + inputFilePath;
//    string moveCommand = "mv " + inputFilePath + ".gz " + inputFilePath;
    system(compressCommand.c_str());
//    system(moveCommand);
}

template <typename DataType, typename DimensionType>
void KDTreeIO<DataType, DimensionType>::unCompress(const string &inputFilePath) {

}

template <typename DataType, typename DimensionType>
KDTree<DataType, DimensionType> KDTreeIO<DataType, DimensionType>::load(const string &modelFilePath) {
    return convertStringVectorToKDTree(loadStringVector(modelFilePath));
}
#endif //KDTREE_KDTREEIO_H
