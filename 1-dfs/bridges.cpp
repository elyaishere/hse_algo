#include "graph.h"
#include "traverse.h"
#include <cstdlib>
#include <iostream>

int main() {
  const size_t size = 5;
  auto gr = Graph<AdjMatrix>::create_indirected_graph(size);

  gr.add_edge(0, 1);
  gr.add_edge(0, 3);
  gr.add_edge(1, 2);
  gr.add_edge(2, 3);
  gr.add_edge(2, 4);

  // 0 - 1
  // |   |
  // 3 - 2
  //    [|]
  //     4

  for (auto [from, to] : find_bridges(gr))
    std::cout << from << '-' << to << std::endl;
}
