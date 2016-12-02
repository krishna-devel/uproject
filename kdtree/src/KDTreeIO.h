#ifndef KDTREE_KDTREEIO_H
#define KDTREE_KDTREEIO_H

#include "KDTree.h"
#include <string>
#include "memory"
#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;

template <typename DataType, typename DimensionType>
class KDTreeIO {
public:
    KDTreeIO();

    static void write(const KDTree<DataType, DimensionType> &kdTree, const string &modelOutputFilePath);
    static KDTree<DataType, DimensionType> load(const string &modelFilePath);
    static Samples<DataType> loadSamples(const string &modelFilePath);

private:
    static vector<string> convertKDTreeToStringVector(const KDTree<DataType, DimensionType> &kdTree);
    static KDTree<DataType, DimensionType> convertStringVectorToKDTree(const vector<string> &stringVector);
    static void compressAndWriteStringVector(const vector <string> &stringVector, const string &outputFilePath);
    static vector<string> decompressAndLoadStringVector(const string &inputFilePath);
    static vector<string> loadStringVector(const string &inputFilePath);
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
                Split<DataType, DimensionType>(
                        node.getSplit()->getSplitDimension(),
                    node.getSplit()->getSplitPoint(),
                    node.getSplit()->getLeftBounds(),
                    node.getSplit()->getRightBounds()
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
void KDTreeIO<DataType, DimensionType>::compressAndWriteStringVector(
        const vector <string> &stringVector,
        const string &outputFilePath
) {
    string stringToWrite;
    for (string s : stringVector) {
        stringToWrite += (s + "bGh88wY3vm2sebBYc");
    }
    Util::writeBinaryFile(outputFilePath, Util::compress(stringToWrite));
}

template <typename DataType, typename DimensionType>
vector<string> KDTreeIO<DataType, DimensionType>::decompressAndLoadStringVector(const string &inputFilePath) {
    vector<string> stringVector;
    string completeString = Util::readBinaryFile(inputFilePath);
    for (string s : Util::split(Util::decompress(completeString), "bGh88wY3vm2sebBYc")) {
        stringVector.push_back(s);
    }
    return stringVector;
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
    else {
        cout << "Unable to open file: " + inputFilePath;
    }

    return stringVector;
}

class no_samples_in_input_file : public exception {};

template <typename DataType, typename DimensionType>
Samples<DataType> KDTreeIO<DataType, DimensionType>::loadSamples(const string &modelFilePath) {
    vector<string> lines = loadStringVector(modelFilePath);
    DimensionType numSamples = lines.size();
    if (numSamples > 0) {
        vector<vector<DataType>> items;
        for(string line : lines) {
            vector<string> tokens = Util::split(line, ",");
            vector<DataType> item;
            for (string token : tokens) {
                item.push_back(stod(token));
            }
            items.push_back(item);
        }
        DimensionType numSamples = items.size();
        DimensionType numDimensions = items[0].size();
        Samples<float> samples {numSamples, numDimensions};
        for(int i = 0; i < numSamples; i++) {
            for(int j = 0; j < numDimensions; j++) {
                samples(i, j) = items[i][j];
            }
        }
        return samples;
    } else {
        throw no_samples_in_input_file();
    }
}

template <typename DataType, typename DimensionType>
void KDTreeIO<DataType, DimensionType>::write(
    const KDTree<DataType, DimensionType> &kdTree,
    const string &modelOutputFilePath
) {
    compressAndWriteStringVector(convertKDTreeToStringVector(kdTree), modelOutputFilePath);
}

template <typename DataType, typename DimensionType>
KDTree<DataType, DimensionType> KDTreeIO<DataType, DimensionType>::load(const string &modelFilePath) {
    return convertStringVectorToKDTree(decompressAndLoadStringVector(modelFilePath));
}
#endif //KDTREE_KDTREEIO_H
