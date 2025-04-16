#include "../../src/rule/rule_node.h"
#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class NodeTest : public testing::Test {
  protected:
    Node addNode = Node(NodeType::kAdd);
    Node removeNode = Node(NodeType::kRemove);
};

TEST_F(NodeTest, ConstructorWorks) {
    EXPECT_EQ(addNode.get_type(), NodeType::kAdd);
    EXPECT_EQ(removeNode.get_type(), NodeType::kRemove);
}
