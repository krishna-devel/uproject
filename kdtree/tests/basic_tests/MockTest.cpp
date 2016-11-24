#include <gmock/gmock.h>
#include <memory>

using testing::Return;
using namespace std;

class I {
public:
//    I(int j) : j(j) {}
    virtual int f1() = 0;
    virtual int f2(int i) = 0;
//    virtual int getJ() = 0;
//private:
//    int j;
};

template <typename T>
class IT {
public:
    virtual int f1() = 0;
    virtual int f2(T i) = 0;
};

class MockI : public I {

public:
//    MockI(int j) : I(j) {}
    MOCK_METHOD0(f1, int());
    MOCK_METHOD1(f2, int(int));
//    MOCK_METHOD1(getJ, int());
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

class N {
public:
    N(int score) : score(score) {}
    int getScore() const { return score; }
    void setScore(int score) { N::score = score; }
    void setLeftN(int v) { N::leftN = unique_ptr<N>(new N(v)); }
    const unique_ptr<N> &getLeftN() const { return leftN; }
private:
    int score;

private:
    unique_ptr<N> leftN;
};

template <typename T>
class Node {
private:
    T value;
    unique_ptr<Node<T>> rightNode;
    unique_ptr<Node<T>> leftNode;
public:
    Node(T value) : value(value) {}
    T getValue() const { return value; }
    unique_ptr<Node<T>> &getLeftNode() { return leftNode; }
    unique_ptr<Node<T>> &getRightNode() { return rightNode; }
    static void updateNode(unique_ptr<Node<T>> &node, T value) {
        node = unique_ptr<Node<int>>(new Node<T>(value));
    }
};

TEST(mock_test, test3) {

    unique_ptr<Node<int>> root;
    // split occurs, set the value of root
    root = unique_ptr<Node<int>>(new Node<int>(15));

    // pass left node down and set a value to it.
    Node<int>::updateNode(root->getLeftNode(), 16);
    Node<int>::updateNode(root->getLeftNode()->getLeftNode(), 17);

    // pass right node down and set a value to it.
    Node<int>::updateNode(root->getRightNode(), 18);
    Node<int>::updateNode(root->getRightNode()->getRightNode(), 19);

    ASSERT_EQ(15, root->getValue());
    ASSERT_EQ(16, root->getLeftNode()->getValue());
    ASSERT_EQ(17, root->getLeftNode()->getLeftNode()->getValue());
    ASSERT_EQ(18, root->getRightNode()->getValue());
    ASSERT_EQ(19, root->getRightNode()->getRightNode()->getValue());

}

TEST(mock_test, test4) {
    N n (10);
    unique_ptr<N> n1(new N(12));
    n.setLeftN(24);

    N n2 (32);
    n2.setScore(56);

    unique_ptr<N> root;

    root = unique_ptr<N>(new N(-1));

    ASSERT_EQ(10, n.getScore());
    ASSERT_EQ(12, n1->getScore());
    ASSERT_EQ(24, n.getLeftN()->getScore());
    ASSERT_EQ(56, n2.getScore());
    ASSERT_EQ(-1, root->getScore());
}

