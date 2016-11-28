#include <gmock/gmock.h>
#include "Util.h"

TEST(UtilTest, replaceAll) {
    string expectedString = ":10;12;l232:";
    string stringInWhichToReplace = "something"+ expectedString +"something";
    Util::replaceAll("something", "", stringInWhichToReplace);
    EXPECT_EQ(stringInWhichToReplace, expectedString);
}

TEST(UtilTest, split) {
    string s = "1::2::3::";
    string delimiter = "::";
    vector<string> expectedVector {"1", "2", "3"};
    EXPECT_EQ(expectedVector, Util::split(s, delimiter));
}

TEST(UtilTest, convertMapToString) {
    map<string, string> m ;
    m["k1"] = "v1";
    m["k2"] = "v2";
    m["k3"] = "v3";
    string expectedString = "k1:v1;k2:v2;k3:v3;";
    EXPECT_EQ(expectedString, Util::convertMapToString(m, ":", ";"));
}

TEST(UtilTest, convertStringToMap) {
    map<string, string> expectedMap;
    expectedMap["k1"] = "v1";
    expectedMap["k2"] = "v2";
    expectedMap["k3"] = "v3";
    EXPECT_EQ(expectedMap, Util::convertStringToMap("k1:v1;k2:v2;k3:v3;", ":", ";"));
}

TEST(UtilTest, convertVectorToStringAndViceVersa) {
    vector<string> v {"s1, s2"};
    EXPECT_EQ(v, Util::convertStringToVector(Util::convertVectorToString(v, ":"), ":"));
}