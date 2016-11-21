#ifndef KDTREE_UTIL_H
#define KDTREE_UTIL_H

#include <random>

using namespace std;

class Util {
public:
    static int getRandomInteger(const int &min, const int &max);
//private:
//    static random_device rd;     // only used once to initialise (seed) engine

};

// Generate random integers from in interval [min, max]:
// http://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
int Util::getRandomInteger(const int &max, const int &min = 0) {
    random_device rd;
    mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    uniform_int_distribution<int> uni(min, max);
    return uni(rng);
}

#endif //KDTREE_UTIL_H
