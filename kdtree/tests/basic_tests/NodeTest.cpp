#include "memory"
#include <gmock/gmock.h>
#include "Node.h"

class NodeTestHelper {
public:
    static void updateNode(unique_ptr<Node<int>> &node, int value) {
        node = unique_ptr<Node<int>>(new Node<int>(value));
    }
};

TEST(NodeTestsad, test_pointers_can_be_set_as_expected) {

    unique_ptr<Node<int>> root;
    // split occurs, set the value of root
    root = unique_ptr<Node<int>>(new Node<int>(15));

    // pass left node down and set a value to it.
    NodeTestHelper::updateNode(root->getLeftNode(), 16);
    NodeTestHelper::updateNode(root->getLeftNode()->getLeftNode(), 17);

    // pass right node down and set a value to it.
    NodeTestHelper::updateNode(root->getRightNode(), 18);
    NodeTestHelper::updateNode(root->getRightNode()->getRightNode(), 19);

    ASSERT_EQ(15, root->getValue());
    ASSERT_EQ(16, root->getLeftNode()->getValue());
    ASSERT_EQ(17, root->getLeftNode()->getLeftNode()->getValue());
    ASSERT_EQ(18, root->getRightNode()->getValue());
    ASSERT_EQ(19, root->getRightNode()->getRightNode()->getValue());

}
