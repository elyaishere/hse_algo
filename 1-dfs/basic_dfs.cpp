#include "graph.h"
#include "traverse.h"
#include "utils.h"
#include <cstdlib>
#include <ctime>

// fills graph randomly
template <typename Graph> void fill(Graph &gr) {
  std::srand(std::time({}));
  auto size = gr.size();

  for (decltype(size) i = 0; i < size; ++i)
    for (decltype(size) j = i + 1; j < size; ++j)
      if (rand() % 2) {
        std::cout << "adding edge: " << i << ' ' << j << std::endl;
        gr.add_edge(i, j);
      }
}

int main() {
  const size_t size = 5;
  auto gr = Graph<AdjMatrix>::create_indirected_graph(size);

  // gr.add_edge(0, 1);
  // gr.add_edge(0, 2);
  // gr.add_edge(1, 3);
  // gr.add_edge(1, 4);

  // draw_tree(gr);

  fill(gr);
  auto edges_to_remove = find_cyclic_edges(gr);
  for (const auto &e : edges_to_remove) {
    std::cout << "removing edge: " << e.first << "->" << e.second << std::endl;
    gr.remove_edge(e);
  }
  draw_tree(gr);
}
