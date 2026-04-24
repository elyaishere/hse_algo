// В Хселэнде располагается N деревень. Некоторые из этих деревень снабжены
// электричеством, остальные – нет. Для упрощения задачи расположим деревни в
// ряд и пронумеруем их от 1 до N слева направо. Пусть i-я деревня описывается
// координатой xi на некоторой прямой. Президент хочет, чтобы электричеством
// были снабжены все деревни. Он решил купить провода для присоединения деревень
// без электричества к каким-либо деревням с электричеством. Президент хочет
// минимизировать расходы на провода, поэтому он хочет знать наименьшую длину
// провода, с которой он сможет провести электричество ко всем деревням.
// Помогите Президенту.

// Первая строка содержит целое число T – количество тестов.
// Далее следует описание тестов в следующем формате:
// Первая строка каждого теста содержит единственное целое число N – количество
// деревень. Вторая строка каждого теста содержит N разделенных пробелами целых
// чисел (0 либо 1). i-е число обозначает есть ли электричество в i-той деревне
// (1 – есть, 0 – нет). Третья строка каждого теста содержит N разделенных
// пробелами целых чисел x1, …, xN – координаты деревень.

// Для каждого тестового случая выведите в отдельную строку единственное целое
// число – наименьшую длину провода, с которой Президент сможет провести
// электричество ко всем деревням.

// 1 <= T <= 10
// Гарантируется, что хотя бы одна деревня снабжена электричеством.
// 1 <= x1 < x2 < ... < xn <= 10^9
// 1 <= N <= 10^5

// 2
// 2
// 01
// 1 2
// 3
// 100
// 1 5 6
// -----
// 1
// 5

#include <climits>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

int calcWiresLength(vector<pair<bool, int>> &villages) {
  auto N = villages.size();
  vector<int> edge_min(N, INT_MAX);
  vector<bool> selected(N, false);
  set<pair<int /* стоимость */, int /* координата */>> q;
  for (int i = 0; i < N; ++i)
    if (villages[i].first) {
      edge_min[i] = 0;
      q.emplace(0, i);
    }

  int total_length = 0;
  for (int i = 0; i < N; ++i) {
    auto it = q.begin();
    int v = it->second;
    selected[v] = true;
    total_length += it->first;
    q.erase(it);

    if (v > 0 && !selected[v - 1]) {
      auto d = villages[v].second - villages[v - 1].second;
      if (edge_min[v - 1] > d) {
        q.erase({edge_min[v - 1], v - 1});
        edge_min[v - 1] = d;
        q.emplace(edge_min[v - 1], v - 1);
      }
    }

    if (v + 1 < N && !selected[v + 1]) {
      auto d = villages[v + 1].second - villages[v].second;
      if (edge_min[v + 1] > d) {
        q.erase({edge_min[v + 1], v + 1});
        edge_min[v + 1] = d;
        q.emplace(edge_min[v + 1], v + 1);
      }
    }
  }

  return total_length;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int T, N;
  cin >> T;
  for (int i = 0; i < T; ++i) {
    cin >> N;
    vector<pair<bool, int>> villages(N);
    cin.get();
    for (auto &v : villages)
      v.first = cin.get() == '1';

    cin.get();
    for (auto &v : villages)
      cin >> v.second;

    std::cout << calcWiresLength(villages) << std::endl;
  }
  return 0;
}
