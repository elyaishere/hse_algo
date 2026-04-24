// Вам дан массив points, представляющий целочисленные координаты некоторых
// точек на плоскости, где points[i] = [xi, yi].

// Стоимость соединения двух точек [xi, yi] и [xj, yj] — это манхэттенское
// расстояние между ними: |xi - xj| + |yi - yj|, где |val| - абсолютное значение
// val.

// Верните минимальную стоимость соединения всех точек.
// Все точки соединены, если между любыми двумя точками существует ровно один
// простой путь.

// Количество точек не превосходит 1000.
// Координаты точек по модулю не превосходят 10^6.
// Все точки в наборе различны.

// https://leetcode.com/problems/min-cost-to-connect-all-points/description/

#include <cassert>
#include <vector>

using namespace std;

int minCostConnectPoints(vector<vector<int>> &points) {
  auto n = points.size();
  vector<vector<int>> adj_matrix(n, vector<int>(n, INT_MAX));
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      auto &c1 = points[i], &c2 = points[j];
      auto dist =
          std::abs(c1.front() - c2.front()) + std::abs(c1.back() - c2.back());
      adj_matrix[i][j] = adj_matrix[j][i] = dist;
    }
  }

  int total_weight = 0;
  vector<bool> selected(n, false);
  vector<int> min_edge(n, INT_MAX);
  min_edge[0] = 0;

  for (int i = 0; i < n; ++i) {
    int v = -1;
    for (int j = 0; j < n; ++j) {
      if (!selected[j] && (v == -1 || min_edge[j] < min_edge[v]))
        v = j;
    }

    if (min_edge[v] == INT_MAX) {
      return -1;
    }

    selected[v] = true;
    total_weight += min_edge[v];

    for (int to = 0; to < n; ++to) {
      if (adj_matrix[v][to] < min_edge[to])
        min_edge[to] = adj_matrix[v][to];
    }
  }

  return total_weight;
}

int main() {
  vector<vector<int>> points = {{0, 0}, {2, 2}, {3, 10}, {5, 2}, {7, 0}};
  auto res = minCostConnectPoints(points);
  assert(res == 20);

  points = {{3, 12}, {-2, 5}, {-4, 1}};
  res = minCostConnectPoints(points);
  assert(res == 18);
}
