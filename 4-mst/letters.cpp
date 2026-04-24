// После прохождения предварительных тестов Стальной алхимик сдаёт свой
// финальный экзамен.

// Полковник Мустанг даёт ему сетку из букв размером n*m.
// Он определяет расстояние между двумя строками сетки как наибольшую абсолютную
// разницу между буквами в одном столбце.

// Стальному алхимику поручено отметить все строки.
// Стоимость отметки первой строки по его выбору равна нулю.
// После этого стоимость отметки каждой последующей строки равна расстоянию
// отмечаемой строки до любой из ранее отмеченных строк.

// Помогите Стальному алхимику определить наименьшее значение наибольшей
// стоимости отметки строки.

// Ввод
// Первая строка содержит размеры сетки n и m.

// Следующие n строк содержат строки по m символов каждая.

// Вывод
// Выведите наименьшее значение наибольшей стоимости отметки строки.

// 2 <= n <= 2000
// 1 <= m <= 25

#include <cassert>
#include <string>
#include <vector>

using namespace std;

int minMaxCost(vector<string> &rows) {
  auto n = rows.size(), m = rows[0].size();
  vector<vector<int>> adj_matrix(n, vector<int>(n, INT_MAX));
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      int max_dist{0};
      for (int k = 0; k < m; ++k) {
        max_dist = std::max(max_dist, abs(rows[i][k] - rows[j][k]));
      }
      adj_matrix[i][j] = adj_matrix[j][i] = max_dist;
    }
  }

  vector<bool> selected(n, false);
  vector<int> min_edge(n, INT_MAX);
  min_edge[0] = 0;
  int mx = 0;

  for (int i = 0; i < n; ++i) {
    int v = -1;
    for (int j = 0; j < n; ++j) {
      if (!selected[j] && (v == -1 || min_edge[j] < min_edge[v]))
        v = j;
    }

    selected[v] = true;
    mx = std::max(mx, min_edge[v]);
    for (int to = 0; to < n; ++to) {
      if (adj_matrix[v][to] < min_edge[to])
        min_edge[to] = adj_matrix[v][to];
    }
  }

  return mx;
}

int main() {
  vector<string> rows = {"abc", "elm", "xet"};
  auto ans = minMaxCost(rows);
  assert(ans == 19);
}
