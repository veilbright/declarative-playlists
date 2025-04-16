#ifndef RULE_NODE_TYPE
#define RULE_NODE_TYPE

#include <map>
#include <string>

enum class NodeType { kAdd = 0, kRemove = 1 };

class NodeTypeStrings {
  public:
    static const std::map<NodeType, std::string> map;
};

#endif
