#ifndef KDTREE_KDTREEIO_H
#define KDTREE_KDTREEIO_H

#include "KDTree.h"
#include <string>
#include "memory"
#include <fstream>
#include <iostream>

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
};

//template <typename DataType, typename DimensionType>
//void KDTreeIO::write(const KDTree<DataType, DimensionType> &kdTree, const string &modelOutputFilePath) {
//    ofstream ofs(modelOutputFilePath, ios::binary);
//    ofs.write((char *)&kdTree, sizeof(kdTree));
//}
//
//template <typename DataType, typename DimensionType>
//KDTree<DataType, DimensionType> KDTreeIO::load(const string &modelFilePath) {
//    KDTree<DataType, DimensionType> kdtree;
//    ifstream ifs(modelFilePath, ios::binary);
//    ifs.read((char *)&two, sizeof(two));
//}



#endif //KDTREE_KDTREEIO_H
