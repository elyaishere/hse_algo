#include "graph.h"
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

template <typename Graph>
std::vector<typename Graph::Edge> find_cyclic_edges(const Graph &graph) {

  std::vector<typename Graph::Edge> cyclic_edges;
  std::vector<uint8_t> visited(graph.size(), 0);

  auto __dfs = [&graph, &cyclic_edges, &visited](
                   this auto &&self, Graph::Vertex v, Graph::Vertex p) -> void {
    // начало обработки вершины
    visited[v] = 1;

    for (auto to : graph.edges(v)) {
      if (!visited[to]) { // вершина не посещена
        self(to, v);
      } else if (to != p && visited[to] == 1) {
        // цикл
        cyclic_edges.emplace_back(v, to);
      }
    }
    // конец обработки вершины
    visited[v] = 2;
  };

  for (typename Graph::Vertex v = 0; v < graph.size(); ++v) {
    if (!visited[v])
      __dfs(v, -1);
  }

  return cyclic_edges;
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
