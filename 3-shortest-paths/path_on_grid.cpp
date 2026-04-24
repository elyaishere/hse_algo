// Дана матрица grid размера MxN (1 <= M,N <= 100).
// Каждая клетка матрицы имеет метку, указывающую на следующую клетку, которую
// следует посетить, если вы сейчас находитесь в этой клетке. Меткой grid[i][j]
// может быть:
// * 1 - это значит, что идти можно в соседнюю справа клетку. (grid[i][j] ->
// grid[i][j + 1])
// * 2 - это значит, что идти можно в соседнюю слева клетку. (grid[i][j] ->
// grid[i][j - 1])
// * 3 - это значит, что идти можно в соседнюю внизу клетку. (grid[i][j] ->
// grid[i + 1][j])
// * 4 - это значит, что идти можно в соседнюю сверху клетку. (grid[i][j] ->
// grid[i - 1][j]) Обратите внимание, что в клетках могут быть метки,
// указывающие за пределы матрицы.

// Движение начинается из верхней левой клетки (0, 0).
// Допустимый путь — это путь, начинающийся из верхней левой клетки (0, 0)
// и заканчивающийся в нижней правой клетке (m - 1, n - 1) в соответствии с
// метками. Допустимый путь не обязательно должен быть кратчайшим.

// Вы можете изменить знак в любой клетке и это будет стоить 1.
// Изменить знак в клетке можно только один раз.

// Требуется вернуть минимальную стоимость, необходимую для того, чтобы
// существовал хотя бы один допустимый путь.

// https://leetcode.com/problems/minimum-cost-to-make-at-least-one-valid-path-in-a-grid/description/

#include <cassert>
#include <deque>
#include <vector>

using namespace std;

int minCost(vector<vector<int>> &grid) {
  auto m = grid.size(), n = grid[0].size();
  const auto grid_size = m * n;
  std::vector<uint16_t> dist(grid_size * 4 /* кол-во типов стрелок */,
                             UINT16_MAX);

  // 0-1 BFS
  std::deque<uint16_t> dq;

  auto idx = (grid[0][0] - 1) * grid_size;
  dist[idx] = 0;
  dq.push_front(idx);

  constexpr int dx[4] = {1, -1, 0, 0};
  constexpr int dy[4] = {0, 0, 1, -1};

  while (!dq.empty()) {
    auto cur = dq.front();
    dq.pop_front();

    auto cur_dist = dist[cur];
    auto arrow_type = cur / grid_size + 1;
    auto pos = cur % grid_size;
    auto x = pos % n;
    auto y = pos / n;

    if (arrow_type == grid[y][x]) {
      for (int i = 0; i <= 3; ++i) {
        if (i + 1 == arrow_type)
          continue;
        // переход в состояние со сменой стрелки в клетке
        auto next_idx = pos + i * grid_size;
        dist[next_idx] = cur_dist + 1;
        // стоимость увеличивается => добавляем в конец,
        // чтобы сохранить сортированность
        dq.push_back(next_idx);
      }
    }

    auto nx = x + dx[arrow_type - 1];
    auto ny = y + dy[arrow_type - 1];

    if (nx >= 0 && nx < n && ny >= 0 && ny < m) {
      auto next_pos = ny * n + nx + (grid[ny][nx] - 1) * grid_size;
      if (cur_dist < dist[next_pos]) {
        // переход согласно стрелке => бесплатно
        dist[next_pos] = cur_dist;
        dq.push_front(next_pos);
      }
    }
  }

  return dist[grid[m - 1][n - 1] * grid_size - 1];
}

int main() {
  vector<vector<int>> grid{
      {1, 1, 1, 1}, {2, 2, 2, 2}, {1, 1, 1, 1}, {2, 2, 2, 2}};
  auto res = minCost(grid);
  assert(res == 3);

  grid = {{1, 1, 3}, {3, 2, 2}, {1, 1, 4}};
  res = minCost(grid);
  assert(res == 0);

  grid = {{1, 2}, {4, 3}};
  res = minCost(grid);
  assert(res == 1);

  grid = {{2, 2, 2}, {2, 2, 2}};
  res = minCost(grid);
  assert(res == 3);

  grid = {{3, 4, 3}, {2, 2, 2}, {2, 1, 1}, {4, 3, 2}, {2, 1, 4}, {2, 4, 1},
          {3, 3, 3}, {1, 4, 2}, {2, 2, 1}, {2, 1, 1}, {3, 3, 1}, {4, 1, 4},
          {2, 1, 4}, {3, 2, 2}, {3, 3, 1}, {4, 4, 1}, {1, 2, 2}, {1, 1, 1},
          {1, 3, 4}, {1, 2, 1}, {2, 2, 4}, {2, 1, 3}, {1, 2, 1}, {4, 3, 2},
          {3, 3, 4}, {2, 2, 1}, {3, 4, 3}, {4, 2, 3}, {4, 4, 4}};
  res = minCost(grid);
  assert(res == 18);
}
