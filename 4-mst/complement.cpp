// Дан неориентированный граф без кратных ребер и петель.
// В нем уже содержится некоторое (возможно, нулевое) количество ребер.
// Можно за определенную плату добавлять в него новые ребра (плата своя для
// каждого ребра). Требуется за наименьшую плату сделать граф связным.

// Входные данные
// В первой строке входных данных содержится одно целое число 1 <= N <= 50 –
// количество вершин в исходном графе. Далее в N строках записано по N
// положительных целых чисел в каждой: j-е число в i-ой строке соответствует
// стоимости добавления ребра, соединяющего вершины i и j, числа не превышают
// 100. В следующих N строках записаны по N чисел, каждое из которых является
// единицей или нулем (1, если вершины соединены, и 0, если не соединены). Обе
// матрицы симметричны.

// Выходные данные
// Вывести единственное число – минимально возможную стоимость дополнения
// данного графа до связного.

// 3
// 0 1 20
// 1 0 10
// 20 10 0
// 0 1 0
// 1 0 0
// 0 0 0
// -----
// 10

#include <climits>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

int calcCost(const vector<vector<int>> &cost,
             const vector<vector<bool>> &adj_matrix) {
  auto N = cost.size();
  vector<bool> selected(N, false);
  vector<int> min_edge(N, INT_MAX);
  set<pair<int /* стоимость */, int /* вершина */>> q;

  min_edge[0] = 0;
  q.emplace(0, 0);

  int total_cost = 0;
  for (int i = 0; i < N; ++i) {
    auto it = q.begin();
    int v = it->second;
    selected[v] = true;
    total_cost += it->first;
    q.erase(it);

    for (int j = 0; j < N; ++j) {
      if (!selected[j]) {
        int edge_cost = adj_matrix[v][j] ? 0 : cost[v][j];
        if (edge_cost < min_edge[j]) {
          q.erase({min_edge[j], j});
          min_edge[j] = edge_cost;
          q.emplace(min_edge[j], j);
        }
      }
    }
  }

  return total_cost;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  cin >> N;
  vector<vector<int>> cost(N, vector<int>(N, 0));
  vector<vector<bool>> adj_matrix(N, vector<bool>(N, false));

  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      cin >> cost[i][j];

  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j) {
      int x;
      cin >> x;
      adj_matrix[i][j] = (x == 1);
    }

  std::cout << calcCost(cost, adj_matrix) << std::endl;
  return 0;
}
