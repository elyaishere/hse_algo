// Даны N точек, занумерованных числами 1, 2, ..., N.
// От каждой точки с меньшим номером к каждой точке с большим номером
// ведет стрелка красного или синего цвета.
// Раскраска стрелок называется однотонной, если нет двух таких точек A и B,
// что от A до B можно добраться как только по красным стрелкам, так и только по
// синим. Ваша задача — по заданной раскраске определить, является ли она
// однотонной. N до 5000.

#include <iostream>
#include <string>
#include <vector>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  bool has_loops = false;
  std::vector<std::vector<uint32_t>> graph;
  std::vector<uint8_t> visited;
  uint32_t N;

  std::cin >> N;
  graph.resize(N);
  visited.resize(N);

  std::string line;
  uint32_t cur = 0;
  for (uint32_t i = 0; i < N - 1; ++i) {
    std::cin >> line;
    for (uint32_t j = 0; j < line.size(); ++j)
      if (line[j] == 'R') {
        // идея: инвертируем красные ребра
        graph[i + j + 1].push_back(i);
      } else {
        graph[i].push_back(i + j + 1);
      }
  }

  // идея: если в графе с инвертированными красными ребрами есть цикл,
  // то он не однотонный
  auto dfs = [&graph, &has_loops, &visited, N](this auto &self,
                                               uint32_t v) -> void {
    visited[v] = 1;

    for (uint32_t to : graph[v]) {
      if (visited[to] == 1) {
        has_loops = true; // нашли цикл
        return;
      }
      if (!visited[to]) {
        self(to);
      }
    }

    visited[v] = 2;
  };

  for (int32_t i = 0; i < N && !has_loops; ++i)
    if (!visited[i])
      dfs(i);

  std::cout << (has_loops ? "NO" : "YES") << std::endl;
}
