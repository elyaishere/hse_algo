// Дано число N (N <= 10^5) и множество десятичных цифр.
// Требуется найти минимальное число, которое делится на N и содержит только
// цифры из заданного множества.

#include <iostream>
#include <queue>
#include <string>
#include <vector>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  uint32_t N;
  std::cin >> N;

  bool digits[10] = {};
  int i;
  while (std::cin >> i)
    digits[i] = true;

  // дерево - остатки при делении на N
  std::vector<bool> visited(N, 0);

  // для восстановления числа запоминаем предыдущую вершину..
  std::vector<uint32_t> parent(N, -1);
  // и цифру
  std::vector<int8_t> digit(N, -1);

  std::queue<uint32_t> q;

  // число не может начинаться с 0
  for (int8_t i = 1; i < 10; ++i) {
    if (digits[i]) {
      auto rem = i % N;
      if (rem == 0) {
        std::cout << i << std::endl;
        return 0;
      }
      if (!visited[rem]) {
        visited[rem] = true;
        parent[rem] = -2;
        digit[rem] = i;
        q.push(rem);
      }
    }
  }

  while (!q.empty()) {
    auto rem = q.front();
    q.pop();

    for (int8_t i = 0; i < 10; ++i) {
      // новый остаток при приписывании цифры i
      auto rem2 = (rem * 10 + i) % N;
      if (digits[i] && !visited[rem2]) {
        visited[rem2] = true;
        parent[rem2] = rem;
        digit[rem2] = i;
        if (rem2 == 0)
          break;
        q.push(rem2);
      }
    }
  }

  if (visited[0]) {
    std::vector<int> ans;
    int8_t cur = 0;
    while (parent[cur] != -2) {
      ans.push_back(digit[cur]);
      cur = parent[cur];
    }
    ans.push_back(digit[cur]);

    for (auto it = ans.rbegin(); it != ans.rend(); ++it)
      std::cout << *it;

    std::cout << std::endl;
  } else {
    std::cout << -1 << std::endl;
  }
}
