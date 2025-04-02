#include "rule_tree.h"

// Outputs formatted representation of Rule Node
void RuleNode::Output(std::ostream &ost, int level) const {
    for (int i = 0; i < level; ++i) {
        ost << "\t";
    }

    // TODO: function to covert NodeType to string, it's used in a lot of places
    ost << "[ ";
    switch (get_type()) {
    case NodeType::kAdd:
        ost << "add";
        break;
    case NodeType::kRemove:
        ost << "remove";
        break;
    }
    ost << " ] ";

    if (!get_subject().artist.empty()) {
        ost << "Artist: " << subject.artist << ", ";
    }
    if (!get_subject().album.empty()) {
        ost << "Album: " << subject.album << ", ";
    }
    if (!get_subject().song.empty()) {
        ost << "Song: " << subject.song << ", ";
    }
    ost << std::endl;
    for (auto it = rules.begin(); it != rules.end(); ++it) {
        it->Output(ost, level + 1);
    }
}

// Outputs formatted representation of Rule Node
void BaseRuleNode::Output(std::ostream &ost) const {
    ost << "Name: " << name << std::endl;
    ost << "Description: " << description << std::endl;
    for (RuleNode node : rules) {
        node.Output(ost);
    }
}

BaseRuleNode::BaseRuleNode(YAML::Node yamlNode) : Node(NodeType::kAdd), rules(yamlNode, {get_type()}) {
    if (yamlNode["name"]) {
        name = yamlNode["name"].as<std::string>();
    }
    if (yamlNode["description"]) {
        description = yamlNode["description"].as<std::string>();
    }
}

std::ostream &operator<<(std::ostream &os, const RuleNode &node) {
    node.Output(os);
    return os;
}
std::ostream &operator<<(std::ostream &os, const BaseRuleNode &node) {
    node.Output(os);
    return os;
}
