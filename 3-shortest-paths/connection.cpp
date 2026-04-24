// В стране Урагания 2 интернет-оператора А и Б.
// В каждом из n городов (2 <= n <= 5000) если либо оператор А, либо оператор Б,
// либо нет интернета. А и Б решили сделать канал связи друг с другом и
// определили m пар городов (1 <= m <= 10^5), которые можно соединить и
// стоимость ci соединения каждого (1 <= ci <= 10^5). Требуется определить
// минимальную стоимость канала, который начинается в городе с оператором А,
// заканчивается в городе с оператором Б (может проходить и через промежуточные
// города) и вывести номер города начала и номер города конца. Если канал
// построить невозможно, вывести -1.

#include <cassert>
#include <climits>
#include <iostream>
#include <optional>
#include <vector>

using namespace std;

struct Connection {
  int from, to, cost;
};

std::optional<Connection> connectionCost(vector<int> &operators,
                                         vector<Connection> &costs) {
  vector<pair<int, int>> dist(operators.size(), {INT_MAX, -1});
  vector<int> visited(operators.size(), false);
  vector<vector<pair<int, int>>> graph(operators.size());

  for (auto &[v, u, c] : costs) {
    graph[v - 1].emplace_back(u - 1, c);
    graph[u - 1].emplace_back(v - 1, c);
  }

  for (int i = 0; i < operators.size(); ++i) {
    if (operators[i] == 1)
      dist[i].first = 0;
  }

  for (int i = 0; i < operators.size(); ++i) {
    int cur = -1;
    for (int j = 0; j < operators.size(); ++j)
      if (!visited[j] && (cur == -1 || dist[j].first < dist[cur].first))
        cur = j;

    if (dist[cur].first == INT_MAX)
      break;
    visited[cur] = true;
    for (auto [v, c] : graph[cur]) {
      if (dist[v].first > dist[cur].first + c) {
        dist[v].first = dist[cur].first + c;
        dist[v].second = (dist[cur].second == -1 ? cur : dist[cur].second);
      }
    }
  }

  auto cur_min = INT_MAX;
  int vertex;
  for (int i = 0; i < operators.size(); ++i) {
    if (operators[i] == 2) {
      if (dist[i].first < cur_min) {
        cur_min = dist[i].first;
        vertex = i;
      }
    }
  }
  if (cur_min == INT_MAX)
    return std::nullopt;
  return Connection{dist[vertex].second + 1, vertex + 1, cur_min};
}

int main() {
  // 0 - нет интернета,  1 - оператор А, 2 - оператор Б
  vector<int> operators{1, 0, 1, 2, 2, 0};
  vector<Connection> costs{
      {1, 3, 3}, {1, 2, 4}, {2, 3, 3}, {2, 4, 2},
      {1, 6, 5}, {3, 5, 6}, {5, 6, 1},
  };
  auto res = connectionCost(operators, costs);
  assert(res.has_value());
  assert(res->from == 3);
  assert(res->to == 4);
  assert(res->cost == 5);

  operators = {1, 0, 0, 2};
  costs = {{1, 3, 3}, {2, 4, 2}};
  res = connectionCost(operators, costs);
  assert(not res.has_value());
}
