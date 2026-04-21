// В этой задаче вам нужно написать код, ищуший выход из подземелья.
// В подземелье есть комнаты и двери между ними.
// Дверь может быть закрыта. Такую дверь сначала нужно открыть ключом.
// Ключи разбросаны по полу в комнатах подземелья. Всего существует 3 различных
// типа ключей. Изначально ключей нет.

// В первой строчке вводится число комнат N. Комнаты занумерованы числами от 0
// до (N-1). Вход в подземелье находится в комнате 0, а выход - в (N - 1).

// Во второй строчке вводится количество ключей K.
// Далее в K строчках вводится номер комнаты и тип ключа (0, 1 или 2), который в
// ней лежит.

// В (K + 3)ой строчке вводится количество открытых дверей M.
// И далее следуют пары (x, y) номеров комнат, двери между которыми открыты.

// Наконец, вводится число закрытых дверей C.
// И далее следуют тройки (x, y, z) номеров комнат и числа от (1 до 7),
// обозначающего, каким набором ключей можно открыть дверь.

// N, M+C <= 10^4
// Нужно вывести "YES", если выход из подземелья возможен, иначе - "NO".

/*
1
0
0
0
----YES
2
0
1
0 1
0
----YES
2
1
0 0
0
1
0 1 1
----YES
3
1
1 0
1
0 1
1
0 2 1
----YES
3
1
1 0
1
0 1
1
1 2 1
----YES
2
0
0
0
----NO
2
1
0 0
0
1
0 1 2
----NO
2
1
1 0
0
1
0 1 1
----NO
4
0
3
0 1
1 2
2 1
0
----NO
*/

#include <iostream>
#include <queue>
#include <string>
#include <vector>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  uint32_t N, K, M, C;
  std::cin >> N >> K;

  // набор ключей = маска, в которой бит соответствует типу ключа
  std::vector<uint32_t> keys(N, 0);
  for (uint32_t i = 0; i < K; ++i) {
    uint32_t room;
    uint32_t key;
    std::cin >> room >> key;
    keys[room] |= (1u << key);
  }

  std::vector<std::vector<uint32_t>> open_doors(N);
  std::cin >> M;
  for (uint32_t i = 0; i < M; ++i) {
    uint32_t r1, r2;
    std::cin >> r1 >> r2;
    open_doors[r1].push_back(r2);
    open_doors[r2].push_back(r1);
  }

  std::vector<std::vector<
      std::pair<uint32_t /* комната */, uint32_t /* набор ключей */>>>
      closed_doors(N);
  std::cin >> C;
  for (uint32_t i = 0; i < C; ++i) {
    uint32_t r1, r2;
    uint32_t k;
    std::cin >> r1 >> r2 >> k;
    closed_doors[r1].emplace_back(r2, k);
    closed_doors[r2].emplace_back(r1, k);
  }

  // вершина в графе - (номер комнаты, набор ключей) => всего N * 8
  std::vector<std::vector<bool>> visited(N, std::vector<bool>(8, false));
  std::queue<std::pair<uint32_t, uint8_t>> q;

  q.push({0 /* нулевая комната */, 0 /* нет ключей */});
  visited[0][0] = true;
  while (!q.empty()) {
    auto [room, old_mask] = q.front();
    q.pop();

    uint32_t new_mask = old_mask | keys[room];

    if (new_mask != old_mask && !visited[room][new_mask]) {
      visited[room][new_mask] = true;
      q.push({room, new_mask});
    }

    for (auto next : open_doors[room]) {
      if (!visited[next][new_mask]) {
        visited[next][new_mask] = true;
        q.push({next, new_mask});
      }
    }

    for (auto [next, door_mask] : closed_doors[room]) {
      if ((new_mask & door_mask) == door_mask && !visited[next][new_mask]) {
        visited[next][new_mask] = true;
        q.push({next, new_mask});
      }
    }
  }

  // по всем возможным маскам для комнаты с выходом
  for (uint32_t mask = 0; mask < 8; mask++) {
    if (visited[N - 1][mask]) {
      std::cout << "YES" << std::endl;
      return 0;
    }
  }

  std::cout << "NO" << std::endl;
}
