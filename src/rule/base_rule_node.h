#ifndef RULE_BASE_RULE_NODE
#define RULE_BASE_RULE_NODE

#include "rule_node.h"
#include "yaml-cpp/node/node.h"
#include <vector>

// Base Node for a tree of Rule Nodes
// Holds metadata and the top list of rules, which all must be add
class BaseRuleNode {
  public:
    BaseRuleNode(std::string name, std::string description) {
    }

    BaseRuleNode(YAML::Node yamlNode); // creates the rule tree for a YAML node

    const std::string &get_name() const {
        return name;
    }
    const std::string &get_description() const {
        return description;
    }

    void PrintTree(std::ostream &ost) const;
    std::vector<RuleNode> rules;

  private:
    std::string name;
    std::string description;
};

std::ostream &operator<<(std::ostream &ost, const BaseRuleNode &node);

#endif
