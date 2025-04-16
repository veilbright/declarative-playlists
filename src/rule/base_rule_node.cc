#include "base_rule_node.h"
#include "node_type.h"
#include "yaml-cpp/yaml.h"

// Outputs formatted representation of Rule Node
void BaseRuleNode::PrintTree(std::ostream &ost) const {
    ost << "Name: " << name << std::endl;
    ost << "Description: " << description << std::endl;
    for (RuleNode node : rules) {
        node.PrintTree(ost);
    }
}

BaseRuleNode::BaseRuleNode(YAML::Node yamlNode) : RuleNode(yamlNode, NodeType::kAdd, {NodeType::kRemove}) {
    if (yamlNode["name"]) {
        name = yamlNode["name"].as<std::string>();
    }
    if (yamlNode["description"]) {
        description = yamlNode["description"].as<std::string>();
    }
}

std::ostream &operator<<(std::ostream &os, const BaseRuleNode &node) {
    node.PrintTree(os);
    return os;
}
