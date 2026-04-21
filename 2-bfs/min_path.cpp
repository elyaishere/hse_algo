#include "../include/graph.h"

#include <iostream>
#include <queue>
#include <vector>

// find min path from src to dst
template <typename Graph>
std::pair<size_t /* длина */, std::vector<typename Graph::Vertex> /* путь */>
MinPath(const Graph &graph, typename Graph::Vertex src,
        typename Graph::Vertex dst) {
  using Vertex = Graph::Vertex;
  std::vector<size_t> dists(graph.size(), -1 /* inf */);
  std::vector<Vertex> parent(graph.size(), -1 /* inf */);
  dists[src] = 0;

  // vanilla bfs
  std::queue<Vertex> queue;
  queue.push(src);
  while (!queue.empty()) {
    auto v = queue.front();
    queue.pop();
    for (auto w : graph.edges(v))
      if (dists[w] == -1) {
        dists[w] = dists[v] + 1;
        parent[w] = v;
        queue.push(w);
      }
  }

  if (dists[dst] == -1)
    return {0uz, {}};

  std::vector<Vertex> path;
  path.resize(dists[dst] + 1);
  path.front() = src;
  Vertex cur = dst;
  for (auto it = path.rbegin(); it != path.rend(); ++it) {
    *it = cur;
    cur = parent[cur];
  }

  return std::make_pair(path.size(), std::move(path));
}

int main() {
  const size_t size = 5;
  auto gr = Graph<AdjList>::create_indirected_graph(size);

  gr.add_edge(0, 1);
  gr.add_edge(0, 4);
  gr.add_edge(1, 2);

  // 0 - 1  3
  // |   |
  // 4   2

  // 4: 2 1 0 4

  auto [dist, path] = MinPath(gr, 2, 4);

  std::cout << "length: " << dist << std::endl;
  for (auto v : path)
    std::cout << v << ' ';

  std::cout << std::endl;
}
