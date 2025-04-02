#include "rule_tree.h"
#include <sstream>

// Outputs formatted representation of Rule Node
void RuleNode::PrintTree(std::ostream &ost, int level) const {
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
    if (!get_subject().track.empty()) {
        ost << "Song: " << subject.track << ", ";
    }
    ost << std::endl;
    for (auto it = rules.begin(); it != rules.end(); ++it) {
        it->PrintTree(ost, level + 1);
    }
}

// Outputs formatted representation of Rule Node
void BaseRuleNode::PrintTree(std::ostream &ost) const {
    ost << "Name: " << name << std::endl;
    ost << "Description: " << description << std::endl;
    for (RuleNode node : rules) {
        node.PrintTree(ost);
    }
}

std::string RuleNode::to_string() const {
    bool is_artist = !get_subject().artist.empty();
    bool is_album = !get_subject().album.empty();
    bool is_track = !get_subject().track.empty();
    std::ostringstream ss;
    if (is_artist) {
        ss << get_subject().artist;
        if (is_album || is_track) {
            ss << " ";
        }
    }
    if (is_album) {
        ss << get_subject().album;
        if (is_track) {
            ss << " ";
        }
    }
    if (is_track) {
        ss << get_subject().track;
    }
    return ss.str();
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
    node.PrintTree(os);
    return os;
}
std::ostream &operator<<(std::ostream &os, const BaseRuleNode &node) {
    node.PrintTree(os);
    return os;
}
