// Индиана Джонс хочет выбраться из лабиринта, который представляет собой граф.
// Для каждого ребра известно время, за которое его можно пробежать, в одной из
// вершин находится герой, в какой-то другой — выход. Чтобы было динамичнее,
// лабиринт постепенно разрушается. Для каждого ребра известно время, в которое
// оно разрушится. Найдите кратчайший путь до выхода или скажите, что все плохо.

#include <algorithm>
#include <cassert>
#include <iostream>
#include <optional>
#include <vector>

using namespace std;

struct Edge {
  int from, to, time_to_run, time_to_destruct;
};

optional<pair<int /* длина */, vector<int> /* путь */>>
temple_of_doom(vector<Edge> &&temple, int n, int start, int finish) {
  vector<bool> visited(n, false);
  vector<pair<int, int>> d(n, {INT_MAX, -1});
  vector<vector<Edge>> graph(n);
  for (auto &edge : temple) {
    graph[edge.from].push_back(edge);
    graph[edge.to].push_back(std::move(edge));
  }

  d[start].first = 0;
  for (int i = 0; i < n; ++i) {
    int cur = -1;
    for (int j = 0; j < n; ++j)
      if (!visited[j] && (cur == -1 || d[j].first < d[cur].first))
        cur = j;
    if (d[cur].first == INT_MAX)
      break;
    visited[cur] = true;
    for (auto &edge : graph[cur]) {
      auto t = d[cur].first + edge.time_to_run;
      if (d[edge.to].first > t && t <= edge.time_to_destruct) {
        d[edge.to].first = t;
        d[edge.to].second = cur;
      }
    }
  }

  if (d[finish].first == INT_MAX)
    return nullopt;

  vector<int> path;
  path.push_back(finish);
  for (int i = finish; d[i].second != -1;) {
    path.push_back(d[i].second);
    i = d[i].second;
  }
  reverse(path.begin(), path.end());
  return make_pair(d[finish].first, std::move(path));
}

int main() {
  vector<Edge> temple{
      {0, 1, 5, 10},
      {1, 2, 3, 10},
  };
  int start = 0, finish = 2, n = 3;
  auto res = temple_of_doom(std::move(temple), n, start, finish);
  assert(res.has_value());
  assert(res->first == 8);
  assert((res->second == vector{0, 1, 2}));

  temple = {{0, 1, 5, 4}, {1, 2, 3, 20}};
  res = temple_of_doom(std::move(temple), n, start, finish);
  assert(not res.has_value());

  n = 4;
  temple = {{0, 1, 2, 5}, {0, 2, 1, 2}, {1, 3, 4, 10}, {2, 3, 2, 10}};
  start = 0, finish = 3;
  res = temple_of_doom(std::move(temple), n, start, finish);
  assert(res.has_value());
  assert(res->first == 3);
  assert((res->second == vector{0, 2, 3}));

  n = 3;
  temple = {{0, 1, 10, 100}, {0, 2, 2, 1}, {2, 1, 1, 100}};
  start = 0, finish = 1;
  res = temple_of_doom(std::move(temple), n, start, finish);
  assert(res.has_value());
  assert(res->first == 10);
  assert((res->second == vector{0, 1}));

  n = 2;
  temple = {
      {0, 1, 5, 10},
  };
  start = 0, finish = 0;
  res = temple_of_doom(std::move(temple), n, start, finish);
  assert(res.has_value());
  assert(res->first == 0);
  assert((res->second == vector{0}));

  n = 2;
  temple = {
      {0, 1, 5, 5},
  };
  start = 0, finish = 1;
  res = temple_of_doom(std::move(temple), n, start, finish);
  assert(res.has_value());
  assert(res->first == 5);
  assert((res->second == vector{0, 1}));

  n = 2;
  temple = {
      {0, 1, 5, 4},
  };
  start = 0, finish = 1;
  res = temple_of_doom(std::move(temple), n, start, finish);
  assert(not res.has_value());

  n = 3;
  temple = {
      {0, 1, 2, 10},
  };
  start = 0, finish = 2;
  res = temple_of_doom(std::move(temple), n, start, finish);
  assert(not res.has_value());

  n = 5;
  temple = {
      {0, 1, 1, 10}, {1, 2, 1, 10},   {2, 3, 1, 10},
      {3, 4, 1, 10}, {0, 4, 20, 100},
  };
  start = 0, finish = 4;
  res = temple_of_doom(std::move(temple), n, start, finish);
  assert(res.has_value());
  assert(res->first == 4);
  assert((res->second == vector{0, 1, 2, 3, 4}));

  n = 4;
  temple = {
      {0, 1, 3, 3},
      {1, 3, 1, 10},
      {0, 2, 1, 2},
      {2, 3, 2, 10},
  };
  start = 0, finish = 3;
  res = temple_of_doom(std::move(temple), n, start, finish);
  assert(res.has_value());
  assert(res->first == 3);
  assert((res->second == vector{0, 2, 3}));
}
