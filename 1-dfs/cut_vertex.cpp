#include "graph.h"
#include "traverse.h"
#include <cstdlib>
#include <iostream>

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
  // 3 - [1]
  //     | \
  //     5 - 4

  for (auto v : find_cut_vertices(gr))
    std::cout << v << std::endl;
}
