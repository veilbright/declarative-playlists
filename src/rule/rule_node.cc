#include "rule_node.h"
#include "node_type.h"
#include "rule_subject.h"
#include "yaml-cpp/node/node.h"
#include "yaml-cpp/node/type.h"
#include "yaml-cpp/yaml.h"
#include <sstream>

// Constructor to build from YAML Node
RuleNode::RuleNode(const YAML::Node yamlNode, const NodeType type, const std::unordered_set<NodeType> bannedTypes)
    : type(type), bannedTypes(bannedTypes) {
    if (yamlNode["artist"]) {
        TrySetString(subject.artist, yamlNode["artist"]);
    }
    if (yamlNode["album"]) {
        TrySetString(subject.album, yamlNode["album"]);
    }
    if (yamlNode["song"]) {
        TrySetString(subject.track, yamlNode["song"]);
    }
    if (yamlNode["track"]) {
        TrySetString(subject.track, yamlNode["track"]);
    }
    for (const auto &[map_type, map_string] : NodeTypeStrings::map) {
        if (bannedTypes.contains(map_type)) { // skip any banned child types
            continue;
        }
        if (yamlNode[map_string]) {
            for (YAML::Node const &subjectNode : yamlNode[map_string]) {
                rules.push_back(RuleNode(subjectNode, map_type));
            }
        }
    }
};

// Used when setting subject fields for new nodes
void RuleNode::TrySetString(std::string &s, const YAML::Node &n) {
    try {
        s = n.as<std::string>();
    } catch (YAML::BadConversion) {
        // TODO: add logging
        throw;
    }
}

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

std::ostream &operator<<(std::ostream &os, const RuleNode &node) {
    node.PrintTree(os);
    return os;
}
