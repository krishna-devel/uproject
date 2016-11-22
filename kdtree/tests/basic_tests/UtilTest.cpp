#include <gtest/gtest.h>
#include <Util.h>

TEST(UtilTest, testGetRandomInteger) {
    for (int i = 0; i < 10000; i++) {
        int randomInt = Util::getRandomInteger(100);
        ASSERT_TRUE(0 <= randomInt);
        ASSERT_TRUE(randomInt <= 100);
    }

    for (int i = 0; i < 10000; i++) {
        int randomInt = Util::getRandomInteger(10, 5);
        ASSERT_TRUE(5 <= randomInt);
        ASSERT_TRUE(randomInt <= 10);
    }
}

//TEST(UtilTest, testMedianOfMedians) {
//    vector<int> items = {1, 2, 3};
//    Util::medianOfMedians(items);
//    int i = 0;
//}

