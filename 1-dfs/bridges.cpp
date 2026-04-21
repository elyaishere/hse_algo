#include "../include/graph.h"

#include <cstdlib>
#include <iostream>
#include <vector>

template <typename Graph>
std::vector<typename Graph::Edge> find_bridges(const Graph &graph) {
  using Vertex = Graph::Vertex;
  using Edge = Graph::Edge;

  auto __graph = graph;
  __graph.set_type(Graph::Type::Directed);

  std::vector<uint8_t> __visited(graph.size(), 0);
  std::vector<size_t> __topsort(graph.size(), 0);
  auto iter = __topsort.rbegin();

  auto __dfs_topsort = [&iter, &__graph, &__visited](this auto &self,
                                                     Vertex v) -> void {
    __visited[v] = true;

    for (auto to : __graph.edges(v)) {
      __graph.remove_edge(to, v);
      if (!__visited[to])
        self(to);
    }

    *iter = v;
    ++iter;
  };

  // первый dfs: топологическая сортировка + удаление ориентированных ребер
  for (Vertex v = 0; v < __graph.size(); ++v)
    if (!__visited[v])
      __dfs_topsort(v);

  // инвертируем ориентированный граф (копия - неэффективно)
  __graph = __graph.invert();

  std::fill(__visited.begin(), __visited.end(), 0);

  auto __reverse_dfs = [&__visited, &__graph](this auto &self, Vertex v,
                                              size_t color) -> void {
    __visited[v] = color;

    for (auto to : __graph.edges(v)) {
      if (!__visited[to]) {
        self(to, color);
      }
    }
  };

  size_t __color = 1;
  // второй dfs: в порядке сортировки ищем сильно связные компоненты
  for (auto v : __topsort) {
    if (!__visited[v]) {
      __reverse_dfs(v, __color);
      ++__color;
    }
  }

  std::vector<Edge> bridges;
  // мост - ребро, соединяющее вершины из разных компонент
  for (Vertex v = 0; v < __graph.size(); ++v) {
    for (auto to : __graph.edges(v)) {
      if (__visited[v] != __visited[to])
        bridges.emplace_back(v, to);
    }
  }

  return bridges;
}

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
