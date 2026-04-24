// Алиса и Боб имеют неориентированный граф с n вершинами и тремя типами ребер:

// Тип 1: Может проходить только Алиса.
// Тип 2: Может проходить только Боб.
// Тип 3: Может проходить как Алиса, так и Боб.
// Дан массив ребер edges, где edges[i] = [type_i, u_i, v_i] представляет собой
// двунаправленное ребро типа type_i между вершинами u_i и v_i. Найдите
// максимальное количество ребер, которые можно удалить, чтобы после удаления
// ребер граф все еще мог быть полностью пройден как Алисой, так и Бобом. Граф
// считается полностью пройденным Алисой и Бобом, если, начиная с любой вершины,
// они могут достичь всех остальных вершин.

// Верните максимальное количество ребер, которые можно удалить, или -1,
// если Алиса и Боб не могут полностью пройти граф.

// 1 <= n <= 10^5
// 1 <= edges.length <= min(10^5, 3 * n * (n - 1) / 2)
// edges[i].length == 3
// 1 <= type_i <= 3
// 1 <= u_i < v_i <= n
// Все ребра (type_i, u_i, v_i) различны.

// https://leetcode.com/problems/remove-max-number-of-edges-to-keep-graph-fully-traversable/description/

#include <cassert>
#include <vector>

using namespace std;

// Disjoint Set Union
struct UnionFind {
  vector<int> parent, rank;
  int counter{0};

  UnionFind(int n) : parent(n), rank(n) {
    for (int i = 0; i < n; i++)
      make_set(i);
  }

  void make_set(int v) {
    parent[v] = v;
    rank[v] = 0;
  }

  int find_set(int v) {
    if (v == parent[v])
      return v;
    return parent[v] = find_set(parent[v]);
  }

  void union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
      if (rank[a] < rank[b])
        swap(a, b);
      parent[b] = a;
      if (rank[a] == rank[b])
        rank[a]++;
    }
    ++counter;
  }
};

int maxNumEdgesToRemove(int n, vector<vector<int>> &edges) {
  UnionFind alice(n), bob(n);
  int res = 0;
  for (auto &e : edges)
    // обрабатываем универсальные рёбра (тип 3)
    if (e[0] == 3) {
      auto v = e[1] - 1;
      auto w = e[2] - 1;
      bool used = false;
      if (alice.find_set(v) != alice.find_set(w)) {
        alice.union_sets(v, w);
        used = true;
      }
      if (bob.find_set(v) != bob.find_set(w)) {
        bob.union_sets(v, w);
        used = true;
      }
      res += !used;
    }

  for (auto &e : edges) {
    if (e[0] == 3)
      continue;
    // обрабатываем оставшиеся рёбра
    UnionFind &target = e[0] == 2 ? bob : alice;
    auto v = e[1] - 1;
    auto w = e[2] - 1;
    bool used = false;
    if (target.find_set(v) != target.find_set(w)) {
      target.union_sets(v, w);
      used = true;
    }
    res += !used;
  }

  if (alice.counter != n - 1 || bob.counter != n - 1)
    return -1;

  return res;
}

int main() {
  vector<vector<int>> edges = {{3, 1, 2}, {3, 2, 3}, {1, 1, 3},
                               {1, 2, 4}, {1, 1, 2}, {2, 3, 4}};
  auto ans = maxNumEdgesToRemove(4, edges);
  assert(ans == 2);

  edges = {{3, 1, 2}, {3, 2, 3}, {1, 1, 4}, {2, 1, 4}};
  ans = maxNumEdgesToRemove(4, edges);
  assert(ans == 0);

  edges = {{3, 2, 3}, {1, 1, 2}, {2, 3, 4}};
  ans = maxNumEdgesToRemove(4, edges);
  assert(ans == -1);
}
