#include "graph.h"

#include <iostream>

template <typename Graph> void print_euler_cycle(const Graph &graph) {
  // при прохождении ребра в dfs удаляем это ребро
  auto __graph = graph;

  auto __dfs = [&__graph](this auto &self, Graph::Vertex v) -> void {
    for (auto to : __graph.edges(v)) {
      __graph.remove_edge(v, to);
      self(to);
    }
    std::cout << v << ' ';
  };

  __dfs(0);
  std::cout << std::endl;
}

int main() {
  const size_t size = 6;
  auto gr = Graph<AdjMatrix>::create_indirected_graph(size);

  gr.add_edge(0, 2);
  gr.add_edge(0, 3);
  gr.add_edge(1, 2);
  gr.add_edge(1, 3);
  gr.add_edge(1, 4);
  gr.add_edge(1, 5);
  gr.add_edge(4, 5);

  // 0 - 2
  // |   |
  // 3 - 1
  //     | \
  //     5 - 4

  print_euler_cycle(gr);
}
