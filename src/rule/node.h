#ifndef RULE_NODE
#define RULE_NODE

#include "node_type.h"

// Parent of all nodes
class Node {
  public:
    Node(NodeType type) : type(type) {
    }
    NodeType const &get_type() const {
        return type;
    }

  private:
    NodeType type;
};

#endif
