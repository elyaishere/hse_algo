#pragma once
#include "graph.h"

#include <deque>
#include <iostream>
#include <set>
#include <string>
#include <vector>

template <typename Graph> void draw_tree(const Graph &tree) {
  if (tree.empty())
    return;

  struct decorated {
    Graph::Vertex v;
    std::string prefix;
    size_t depth;
    bool is_last;
  };

  std::set<typename Graph::Vertex> seen;
  for (auto i = 0uz; i < tree.size(); ++i) {
    if (seen.contains(i))
      continue;
    std::deque<decorated> vertices;
    vertices.emplace_back(i, "", 0, true);

    while (!vertices.empty()) {
      auto [v, prefix, depth, is_last] = vertices.front();
      vertices.pop_front();

      if (seen.contains(v))
        continue;
      seen.insert(v);

      if (v)
        std::cout << prefix << "|" << std::endl;

      prefix += (is_last ? "|___ " : "|--- ");

      std::cout << prefix;
      std::cout << v << "[" << depth << "]" << std::endl;

      std::vector<typename Graph::Vertex> children;
      for (auto to : tree.edges(v)) {
        if (!seen.contains(to)) {
          children.push_back(to);
        }
      }

      prefix = std::string(prefix.size(), ' ');

      for (auto i = std::ssize(children) - 1; i >= 0; --i) {
        bool last_child = i == 0;
        vertices.emplace_back(children[i], prefix, depth + 1, last_child);
      }
    }
  }
}
