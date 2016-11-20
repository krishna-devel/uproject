#ifndef KDTREE_MATRIXXDADAPTER_H
#define KDTREE_MATRIXXDADAPTER_H

#include "eigen3/Eigen/Dense"

using namespace std;
using Eigen::MatrixXd;


class MatrixXdAdapter {

public:
    MatrixXdAdapter(const MatrixXd &matrix);

    int rows();

private:
    MatrixXd matrix;
};

#endif //KDTREE_MATRIXXDADAPTER_H
