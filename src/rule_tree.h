#ifndef RULE_TREE
#define RULE_TREE

#include "node_list.h"
#include "rule_node.h"

class RuleTree {
public:
  NodeList<RuleNode> rules;
};

#endif
