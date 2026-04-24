// Дана сеть из N узлов, пронумерованных от 1 до N.
// Также даны значения times — список времен прохождения сигнала в виде
// направленных ребер: times[i] = (ui, vi, wi), где ui — исходный узел, vi —
// целевой узел, а wi — время, необходимое для распространения сигнала от ui к
// vi.

// Сигнал отправляется с заданного узла k.
// Верните минимальное время, необходимое для того, чтобы все N узлов приняли
// сигнал. Если невозможно, верните -1.

// https://leetcode.com/problems/network-delay-time/description/

#include <cassert>
#include <numeric>
#include <vector>

using namespace std;

int networkDelayTime(vector<vector<int>> &times, int n, int k) {
  vector<vector<pair<int, int>>> graph(n);
  for (auto &triplet : times) {
    graph[triplet[0] - 1].emplace_back(triplet[1] - 1, triplet[2]);
  }

  vector<bool> visited(graph.size(), false);
  vector<int> dist(graph.size(), INT_MAX);
  dist[k - 1] = 0;
  for (int i = 0; i < graph.size(); ++i) {
    int cur = -1;
    for (int j = 0; j < graph.size(); ++j)
      if (!visited[j] && (cur == -1 || dist[j] < dist[cur]))
        cur = j;

    if (dist[cur] == INT_MAX)
      break;
    visited[cur] = true;
    for (auto [to, d] : graph[cur])
      if (dist[to] > dist[cur] + d) {
        dist[to] = dist[cur] + d;
      }
  }

  auto maximum = reduce(dist.begin(), dist.end(), 0,
                        [](auto a, auto b) { return max(a, b); });

  return maximum == INT_MAX ? -1 : maximum;
}

int main() {
  vector<vector<int>> times{{2, 1, 1}, {2, 3, 1}, {3, 4, 1}};
  auto res = networkDelayTime(times, 4, 2);
  assert(res == 2);

  times = {{1, 2, 1}};
  res = networkDelayTime(times, 2, 1);
  assert(res == 1);

  times = {{1, 2, 1}};
  res = networkDelayTime(times, 2, 2);
  assert(res == -1);
}
