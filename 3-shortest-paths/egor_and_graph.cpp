// У Егора есть взвешенный ориентированный граф, состоящий из n вершин.
// В этом графе между любой парой различных вершин есть ребро в обоих
// направлениях. Егор любит играть с графом, и сейчас он придумал новую игру:
//  * Игра состоит из n шагов.
//  * На i-том шаге Егор удаляет из графа вершину номер xi.
//  * Удаляя вершину, Егор удаляет все ребра, которые входили в данную вершину и
//  которые выходили из нее.
// Перед выполнением каждого шага,
// Егор хочет знать сумму длин кратчайших путей между всеми парами оставшихся
// вершин. Кратчайший путь может проходить через любую оставшуюся вершину.
// Помогите Егору, выведите значение искомой суммы перед каждым шагом.

#include <algorithm>
#include <cassert>
#include <vector>

using namespace std;

vector<uint64_t> solution(vector<vector<uint64_t>> &&graph,
                          vector<uint32_t> &&seq) {
  // будем не удалять вершины, а добавлять
  reverse(seq.begin(), seq.end());

  auto n = graph.size();
  // матрица расстояний
  vector<vector<uint64_t>> dist(n, vector<uint64_t>(n, INT_MAX));
  for (uint32_t i = 0; i < n; i++) {
    dist[i][i] = 0;
  }

  vector<bool> added(n, false);
  vector<uint64_t> answers(n);
  uint64_t total = 0;

  // добавляем вершины в обратном порядке
  for (uint32_t step = 0; step < n; step++) {
    auto cur = seq[step] - 1;
    added[cur] = true;

    // инициализируем расстояния от/до новой вершины
    // используем прямые ребра из исходного графа
    for (uint32_t u = 0; u < n; u++) {
      if (added[u]) {
        dist[cur][u] = graph[cur][u];
        dist[u][cur] = graph[u][cur];
      }
    }

    // уточняем расстояния до/от cur через уже добавленные вершины
    for (uint32_t k = 0; k < n; k++) {
      if (added[k] && k != cur) {
        for (uint32_t u = 0; u < n; u++) {
          if (added[u]) {
            if (dist[u][cur] > dist[u][k] + dist[k][cur]) {
              dist[u][cur] = dist[u][k] + dist[k][cur];
            }
            if (dist[cur][u] > dist[cur][k] + dist[k][u]) {
              dist[cur][u] = dist[cur][k] + dist[k][u];
            }
          }
        }
      }
    }
    // обновляем расстояния между всеми парами через новую вершину cur
    for (uint32_t u = 0; u < n; u++) {
      if (added[u]) {
        for (uint32_t v = 0; v < n; v++) {
          if (added[v]) {
            if (dist[u][v] > dist[u][cur] + dist[cur][v]) {
              dist[u][v] = dist[u][cur] + dist[cur][v];
            }
          }
        }
      }
    }

    // считаем сумму для текущего множества добавленных вершин
    total = 0;
    for (uint32_t u = 0; u < n; u++) {
      if (added[u]) {
        for (uint32_t v = 0; v < n; v++) {
          if (added[v] && u != v) {
            total += dist[u][v];
          }
        }
      }
    }

    // сохраняем ответ (в обратном порядке, так как мы идем с конца)
    answers[n - 1 - step] = total;
  }

  return answers;
}

int main() {
  vector<vector<uint64_t>> graph{
      {0},
  };
  vector<uint32_t> seq{1};
  auto res = solution(std::move(graph), std::move(seq));
  vector<uint64_t> expected{0};
  assert(res == expected);

  graph = {
      {0, 5},
      {4, 0},
  };
  seq = {1, 2};
  res = solution(std::move(graph), std::move(seq));
  expected = {9, 0};
  assert(res == expected);

  graph = {
      {0, 3, 1, 1},
      {6, 0, 400, 1},
      {2, 4, 0, 1},
      {1, 1, 1, 0},
  };
  seq = {4, 1, 2, 3};
  res = solution(std::move(graph), std::move(seq));
  expected = {17, 23, 404, 0};
  assert(res == expected);
}
