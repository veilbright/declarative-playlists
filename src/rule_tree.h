#ifndef RULE_TREE
#define RULE_TREE

#include "rule_subject.cc"
#include "yaml-cpp/yaml.h"
#include <initializer_list>
#include <iostream>
#include <unordered_set>

enum class NodeType { kAdd, kRemove };

// Parent of all nodes
class Node {
public:
  Node(NodeType type) : type(type) {}
  NodeType const &get_type() const { return type; }

private:
  NodeType type;
};

// List of nodes and what type of nodes are allowed in the list
template <typename T> class NodeLevel {
public:
  // Contructor to control the types of Rules allowed in the level
  // Add types allow Remove types, and vice versa
  NodeLevel(std::initializer_list<NodeType> currentTypes) {
    ConfigAllowedTypes(currentTypes);
  }
  // Constructor to set one allowed type
  NodeLevel(NodeType type) { allowedChildTypes.insert(type); }

  // Constructor to build from YAML Node
  NodeLevel(YAML::Node yamlNode, std::unordered_set<NodeType> currentTypes) {
    ConfigAllowedTypes(currentTypes);

    for (NodeType type : currentTypes) {
      YAML::Node nodeSeq;
      switch (type) {
      case NodeType::kAdd:
        nodeSeq = yamlNode["add"];
        break;
      case NodeType::kRemove:
        nodeSeq = yamlNode["remove"];
        break;
      }
      // Check if nodeSeq was set and the node actually exists
      if (nodeSeq) {
        for (YAML::Node const &child : nodeSeq) {
          RuleSubject subject;

          if (child["artist"]) {
            TrySetString(subject.artist, child["artist"]);
          }
          if (child["album"]) {
            TrySetString(subject.album, child["album"]);
          }
          if (child["song"]) {
            TrySetString(subject.song, child["song"]);
          }
          T node = T(type, subject);
          node.rules = NodeLevel<T>(child, allowedChildTypes);
          push_back(node);
        }
      }
    }
  }

  std::list<T>::const_iterator const begin() const { return nodes.begin(); }
  std::list<T>::const_iterator const end() const { return nodes.end(); }
  std::size_t const size() const { return nodes.size(); }
  void push_back(T node) { nodes.push_back(node); }

private:
  std::unordered_set<NodeType> allowedChildTypes;
  std::list<T> nodes;
  void ConfigAllowedTypes(std::unordered_set<NodeType> currentTypes) {
    for (NodeType type : currentTypes) {
      // Each current type can allow more types
      switch (type) {
      case NodeType::kAdd:
        allowedChildTypes.insert(NodeType::kRemove);
        break;
      case NodeType::kRemove:
        allowedChildTypes.insert(NodeType::kAdd);
        break;
      } // default is no allowedTypes
    }
  }

  // Used when setting subject fields for new nodes
  void TrySetString(std::string &s, const YAML::Node &n) {
    try {
      s = n.as<std::string>();
    } catch (YAML::BadConversion) {
      // TODO: add logging
      std::cout << "Bad Conversion to string\n";
    }
  }
};

// Implements functionality for Add and Remove Node classes
class RuleNode : Node {
public:
  RuleNode(NodeType type, RuleSubject subject)
      : Node(type), subject(subject), rules({type}) {}

  RuleNode(YAML::Node yamlNode);

  RuleSubject const &get_subject() const { return subject; }
  void Output(std::ostream &ost, int level = 0) const;

  NodeLevel<RuleNode> rules;

private:
  RuleSubject subject;
};

// Base Node for a tree of Rule Nodes
// Holds metadata and the top list of rules
class BaseRuleNode : Node {
public:
  BaseRuleNode(std::string name, std::string description)
      : Node(NodeType::kAdd), name(name), description(description),
        rules(NodeType::kAdd) {}

  BaseRuleNode(YAML::Node yamlNode);

  void Output(std::ostream &ost) const;

  NodeLevel<RuleNode> rules;

private:
  std::string name;
  std::string description;
};

#endif
