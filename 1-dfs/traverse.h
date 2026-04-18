#pragma once
#include "graph.h"

#include <vector>

template <typename Graph>
std::vector<typename Graph::Vertex> find_cut_vertices(const Graph &graph) {
  std::vector<typename Graph::Vertex> cut_vertices;
  std::vector<size_t> cut_depths(graph.size(), 0);

  auto __dfs = [&graph, &cut_depths, &cut_vertices](
                   this auto &self, Graph::Vertex v, size_t depth) -> size_t {
    cut_depths[v] = depth;
    size_t min_cut(-1); // inf
    bool is_cut = false;
    size_t children{0};
    for (auto to : graph.edges(v)) {
      if (cut_depths[to] == 0) { // тут ещё не были
        ++children;
        auto tmp = self(to, depth + 1);
        min_cut = std::min(min_cut, tmp);
        if (depth > 1 && tmp >= depth)
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

template <typename Graph>
std::vector<typename Graph::Edge> find_cyclic_edges(const Graph &graph) {
  std::vector<typename Graph::Edge> cyclic_edges;
  auto on_loop = [&cyclic_edges](auto from, auto to) {
    cyclic_edges.emplace_back(from, to);
  };

  graph.traverse_dfs(std::move(on_loop));
  return cyclic_edges;
}

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

  for (Vertex v = 0; v < __graph.size(); ++v)
    if (!__visited[v])
      __dfs_topsort(v);

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
  for (auto v : __topsort) {
    if (!__visited[v]) {
      __reverse_dfs(v, __color);
      ++__color;
    }
  }

  std::vector<Edge> bridges;
  for (Vertex v = 0; v < __graph.size(); ++v) {
    for (auto to : __graph.edges(v)) {
      if (__visited[v] != __visited[to])
        bridges.emplace_back(v, to);
    }
  }

  return bridges;
}
