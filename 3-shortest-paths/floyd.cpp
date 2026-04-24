// Полный ориентированный взвешенный граф задан матрицей смежности.
// Постройте матрицу кратчайших путей между его вершинами.
// Гарантируется, что в графе нет циклов отрицательного веса.
// N (1 <= N <= 100) – количество вершин графа
// все веса по модулю не превосходят 100

#include <cassert>
#include <climits>
#include <vector>

using namespace std;

vector<vector<int>> floyd(vector<vector<int>> &&graph) {
  vector<vector<int>> dist(graph.size(), vector<int>(graph.size(), INT_MAX));
  for (int i = 0; i < graph.size(); ++i) {
    dist[i][i] = 0;
    for (int j = 0; j < graph.size(); ++j) {
      if (graph[i][j])
        dist[i][j] = graph[i][j];
    }
  }

  for (int i = 0; i < graph.size(); ++i)
    for (int j = 0; j < graph.size(); ++j)
      for (int k = 0; k < graph.size(); ++k)
        if (dist[j][k] > dist[j][i] + dist[i][k])
          dist[j][k] = dist[j][i] + dist[i][k];
  return dist;
}

int main() {
  vector<vector<int>> graph{
      {0, 5, 9, 100},
      {100, 0, 2, 8},
      {100, 100, 0, 7},
      {4, 100, 100, 0},
  };
  auto res = floyd(std::move(graph));
  vector<vector<int>> expected = {
      {0, 5, 7, 13},
      {12, 0, 2, 8},
      {11, 16, 0, 7},
      {4, 9, 11, 0},
  };
  assert((res == expected));
}