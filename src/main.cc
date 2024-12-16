#include "rule_tree.h"
#include <iostream>
#include <string>

int main() {
  std::string s = "test/alternate.yaml";
  YAML::Node node = YAML::LoadFile(s);
  BaseRuleNode rn = BaseRuleNode(node);
  rn.Output(std::cout);
}
