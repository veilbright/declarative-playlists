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
    for (RuleNode node : rules) {
        node.Output(ost);
    }
}

BaseRuleNode::BaseRuleNode(YAML::Node yamlNode)
    : Node(NodeType::kAdd), rules(yamlNode, {get_type()}) {
    // TODO: set name and description from YAML
}
