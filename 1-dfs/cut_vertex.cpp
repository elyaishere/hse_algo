#include "graph.h"

#include <cstdlib>
#include <iostream>

template <typename Graph>
std::vector<typename Graph::Vertex> find_cut_vertices(const Graph &graph) {

  std::vector<typename Graph::Vertex> cut_vertices;
  std::vector<size_t> cut_depths(graph.size(), 0);

  auto __dfs = [&graph, &cut_depths, &cut_vertices](
                   this auto &self, Graph::Vertex v, size_t depth) -> size_t {
    cut_depths[v] = depth; // текущая глубина вершины
    size_t min_cut(-1);    // inf
    bool is_cut = false;
    size_t children{0};
    for (auto to : graph.edges(v)) {
      if (cut_depths[to] == 0) { // тут ещё не были
        ++children;
        auto tmp = self(to, depth + 1); // минимум для вершины to
        min_cut = std::min(min_cut, tmp);
        if (depth > 1 && tmp >= depth) // есть обходное ребро
          is_cut = true;
      } else
        min_cut = std::min(min_cut, cut_depths[to]);
    }

    if (depth == 1 && children >= 2)
      // корень - точка сочленения, если у него хотя бы 2 потомка в dfs
      is_cut = true;

    if (is_cut)
      cut_vertices.push_back(v);

    return min_cut;
  };

  for (typename Graph::Vertex v = 0; v < graph.size(); ++v)
    if (!cut_depths[v])
      __dfs(v, 1);

  return cut_vertices;
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
  // 3 - [1]
  //     | \
  //     5 - 4

  for (auto v : find_cut_vertices(gr))
    std::cout << v << std::endl;
}
