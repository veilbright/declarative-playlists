#include "rule_node.h"
#include <string>

int main() {
  std::string s = "test/alternate.yaml";
  YAML::Node node = YAML::LoadFile(s);
  RuleNode rn = RuleNode(node);
  /*RuleSubject subject = rn.get_subject();*/
  /*std::cout << "Artist: " << subject.album << std::endl;*/
}
