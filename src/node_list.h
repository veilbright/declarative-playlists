#ifndef NODE_LIST
#define NODE_LIST

#include <forward_list>

template <typename T> class NodeList {
public:
  void add_node(T node);
  const std::forward_list<T> &get_nodes();

private:
  std::forward_list<T> nodes;
};

#endif
