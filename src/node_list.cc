#include "node_list.h"

template <typename T> const std::forward_list<T> &NodeList<T>::get_nodes() {
  return nodes;
}

template <typename T> void NodeList<T>::add_node(T node) {
  nodes.push_back(node);
}
