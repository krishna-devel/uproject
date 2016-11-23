#include <gmock/gmock.h>

using testing::Return;

class I {
public:
    virtual int f1() = 0;
    virtual int f2(int i) = 0;
};

template <typename T>
class IT {
public:
    virtual int f1() = 0;
    virtual int f2(T i) = 0;
};

class MockI : public I {
public:
    MOCK_METHOD0(f1, int());
    MOCK_METHOD1(f2, int(int));
};

template <typename T>
class MockIT : public IT<T> {
public:
    MOCK_METHOD0_T(f1, int());
    MOCK_METHOD1_T(f2, int(T));
};

TEST(mock_test, test1) {
    MockI i;
    EXPECT_CALL(i, f1()).WillOnce(Return(100));
    EXPECT_CALL(i, f2(10)).WillOnce(Return(10));

    ASSERT_EQ(100, i.f1());
    ASSERT_EQ(10, i.f2(10));
}

TEST(mock_test, test2) {
    MockIT<int> i;
    EXPECT_CALL(i, f1()).WillOnce(Return(100));
    EXPECT_CALL(i, f2(10)).WillOnce(Return(10));

    ASSERT_EQ(100, i.f1());
    ASSERT_EQ(10, i.f2(10));
}

