#ifndef KDTREE_KDTREEIO_H
#define KDTREE_KDTREEIO_H

#include "KDTree.h"
#include <string>
#include "memory"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "zlib.h"

using namespace std;

template <typename DataType, typename DimensionType>
class KDTreeIO {
public:
    static void write(const KDTree<DataType, DimensionType> &kdTree, const string &modelOutputFilePath);
    static KDTree<DataType, DimensionType> load(const string &modelFilePath);

private:
    static vector<string> convertKDTreeToStringVector(const KDTree<DataType, DimensionType> &kdTree);
    static KDTree<DataType, DimensionType> convertStringVectorToKDTree(const vector<string> &stringVector);
    static void writeStringVector(const vector<string> &stringVector, const string &outputFilePath);
    static vector<string> loadStringVector(const string &inputFilePath);
    static string compressString(const string &input);
    static string unCompressString(const string &input);
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
                    node.getDimensionWithSplitInfo()->getSplitDimension(),
                    node.getDimensionWithSplitInfo()->getSplitPoint(),
                    node.getDimensionWithSplitInfo()->getLeftBounds(),
                    node.getDimensionWithSplitInfo()->getRightBounds()
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

/*
* https://gist.github.com/arq5x/5315739
*/
template <typename DataType, typename DimensionType>
string KDTreeIO<DataType, DimensionType>::compressString(const string &input) {
    char output[input.size()+50];

    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    // setup "a" as the input and "output" as the compressed output
    defstream.avail_in = (uInt)strlen(input.c_str())+1; // size of input, string + terminator
    defstream.next_in = (Bytef *)input.c_str(); // input char array
    defstream.avail_out = (uInt)sizeof(output); // size of output
    defstream.next_out = (Bytef *)output; // output char array

    // the actual compression work.
    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);

    string returnStr(output);
    return returnStr;
};

template <typename DataType, typename DimensionType>
string KDTreeIO<DataType, DimensionType>::unCompressString(const string &input) {
    char output[input.size()+500];

    z_stream infstream;
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    // setup "b" as the input and "c" as the compressed output
    infstream.avail_in = (uInt)(strlen(input.c_str())+1); // size of input
    infstream.next_in = (Bytef *)input.c_str(); // input char array
    infstream.avail_out = (uInt)sizeof(output); // size of output
    infstream.next_out = (Bytef *)output; // output char array

    // the actual DE-compression work.
    inflateInit(&infstream);
    inflate(&infstream, Z_NO_FLUSH);
    inflateEnd(&infstream);

    string returnStr(output);
    return returnStr;
};


template <typename DataType, typename DimensionType>
void KDTreeIO<DataType, DimensionType>::write(
    const KDTree<DataType, DimensionType> &kdTree,
    const string &modelOutputFilePath
) {
    writeStringVector(convertKDTreeToStringVector(kdTree), modelOutputFilePath);
}

template <typename DataType, typename DimensionType>
KDTree<DataType, DimensionType> KDTreeIO<DataType, DimensionType>::load(const string &modelFilePath) {
    return convertStringVectorToKDTree(loadStringVector(modelFilePath));
}
#endif //KDTREE_KDTREEIO_H
