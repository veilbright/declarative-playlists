#ifndef RULE_RULE_LEVEL
#define RULE_RULE_LEVEL

#include "node.h"
#include "node_type.h"
#include "rule_subject.h"
#include "yaml-cpp/node/node.h"
#include <iostream>
#include <unordered_set>

// Implements functionality for Add and Remove Node classes
class RuleNode : public Node {
  public:
    RuleNode(const NodeType type, const RuleSubject subject) : Node(type), subject(subject) {
    }

    RuleNode(const NodeType type) : RuleNode(type, {}) {
    }

    // Constructor to build from YAML Node with disallowed types
    RuleNode(const YAML::Node yamlNode, const NodeType type, const std::unordered_set<NodeType> bannedTypes);

    // Constructor to build from YAML Node
    RuleNode(const YAML::Node yamlNode, const NodeType type) : RuleNode(yamlNode, type, {}) {
    }

    RuleSubject const &get_subject() const {
        return subject;
    }

    void PrintTree(std::ostream &ost, int level = 0) const; // outputs a readable rule tree to ost
    std::string to_string() const;
    std::vector<RuleNode> rules;

  private:
    RuleSubject subject;
    std::unordered_set<NodeType> bannedTypes;

    // Used when setting subject fields for new nodes
    void TrySetString(std::string &s, const YAML::Node &n);
};

// allows the use of the stream operator instead of Output function
std::ostream &operator<<(std::ostream &ost, const RuleNode &node);

#endif
