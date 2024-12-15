#ifndef RULE_NODE
#define RULE_NODE

#include "node_list.h"
#include "rule_subject.cc"
#include "yaml-cpp/yaml.h"

enum class RuleType { kAdd, kRemove };

// Implements functionality for Add and Remove Node classes
class RuleNode {
public:
  RuleNode(RuleSubject subject);

  RuleNode(YAML::Node yamlNode);
  RuleSubject get_subject();

  NodeList<RuleNode> rules;

private:
  RuleSubject subject;
};

#endif
