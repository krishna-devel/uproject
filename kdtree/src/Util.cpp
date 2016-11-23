#include "Util.h"

// Generate random integers from in interval [min, max]:
// http://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
int Util::getRandomInteger(const int &max, const int &min) {
    random_device rd;
    mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    uniform_int_distribution<int> uni(min, max);
    return uni(rng);
}
