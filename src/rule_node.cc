#include "rule_node.h"
#include <iostream>

RuleNode::RuleNode(RuleSubject subject) { this->subject = subject; }

RuleNode::RuleNode(YAML::Node yamlNode) {
  // Don't add rules if they aren't defined
  YAML::Node yamlAdd = yamlNode["add"];
  if (!yamlAdd) {
    std::cout << "No rules provided\n";
    return;
  }
  for (YAML::const_iterator it = yamlAdd.begin(); it != yamlAdd.end(); ++it) {
    if ((*it)["artist"]) {
      std::cout << "Artist: " << (*it)["artist"] << std::endl;
    }
  }
}

RuleSubject RuleNode::get_subject() { return subject; }
