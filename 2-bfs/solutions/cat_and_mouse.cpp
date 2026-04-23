// Мышонок и Кот играют в... кошки-мышки. Игроки ходят по очереди.

// Мышонок начинает ход с вершины 1 и ходит первым,
// Кот начинает ход с вершины 2 и ходит вторым, а в вершине 0 находится норка.

// Во время хода каждого игрока он должен пройти по одному ребру графа, которое
// выходит из текущей вершины. Кроме того, Коту не разрешается находиться в
// вершине 0.

// Таким образом, игра может закончиться тремя способами:
// (0) Если позиция повторяется (т.е. игроки находятся в той же позиции, что и в
// предыдущий ход, и ход ходит тот же игрок), игра заканчивается ничьей. (1)
// Если Мышонок когда-либо достигнет норки, Мышонок выигрывает. (2) Если Кот
// когда-либо окажется в той же вершине, что и Мышонок, Кот выигрывает.

// 3 <= Кол-во вершин <= 50

// Input: graph = [[2,5],[3],[0,4,5],[1,4,5],[2,3],[0,2,3]]
// Output: 0
// Input: graph = [[1,3],[0],[3],[0,2]]
// Output: 1

// https://leetcode.com/problems/cat-and-mouse/description/

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

int catMouseGame(std::vector<std::vector<int>> &graph) {
  enum { DRAW, MOUSE, CAT };
  using Vertex = std::tuple<int, /* mouse pos */
                            int, /* cat pos */
                            int /* player */>;

  auto nodes_cnt = graph.size();

  // [mouse pos][cat pos][player] -> who wins
  int play_graph[50][50][2]{};
  // [mouse pos][cat pos][player] -> number of possible moves
  int degree[50][50][2];

  bool connected_with_zero[50]{};
  for (int i = 0; i < graph.size(); ++i) {
    connected_with_zero[i] = std::ranges::find(graph[i], 0) != graph[i].end();
  }

  std::queue<Vertex> q;

  // initial
  for (int i = 0; i < graph.size(); ++i) {
    // when the mouse is at position 0, the mouse wins regardless of the cat's
    // position
    play_graph[0][i][0] = play_graph[0][i][1] = MOUSE;
    q.emplace(0, i, MOUSE);
    q.emplace(0, i, CAT);
    if (i > 0) {
      // when the mouse and the cat are at the same position, the cat wins
      play_graph[i][i][0] = play_graph[i][i][1] = CAT;
      q.emplace(i, i, MOUSE);
      q.emplace(i, i, CAT);
    }

    for (int j = 0; j < graph.size(); ++j) {
      // the mouse is free to move to any connected node
      degree[i][j][0] = graph[i].size();
      // the cat cannot move to 0
      degree[i][j][1] = graph[j].size() - int(connected_with_zero[j]);
    }
  }

  while (!q.empty()) {
    auto [mpos, cpos, turn] = q.front();
    q.pop();

    // moving from the bottom
    auto winner = play_graph[mpos][cpos][turn - 1];
    if (turn == MOUSE) { // the mouse plays
      // resolving the cat's position on the previous move
      for (auto prev_cpos : graph[cpos]) {
        if (prev_cpos == 0)
          continue;
        if (play_graph[mpos][prev_cpos][1] == DRAW) {
          if (winner == CAT) {
            // the cat can make a winner move
            play_graph[mpos][prev_cpos][1] = CAT;
            q.emplace(mpos, prev_cpos, CAT);
          } else if (--degree[mpos][prev_cpos][1] == 0) {
            play_graph[mpos][prev_cpos][1] = MOUSE;
            q.emplace(mpos, prev_cpos, CAT);
          }
        }
      }
    } else { // the cat plays
      // resolving the mouse's position on the previous move
      for (auto prev_mpos : graph[mpos]) {
        if (play_graph[prev_mpos][cpos][0] == DRAW) {
          if (winner == MOUSE) {
            // the mouse can make a winner move
            play_graph[prev_mpos][cpos][0] = MOUSE;
            q.emplace(prev_mpos, cpos, MOUSE);
          } else if (--degree[prev_mpos][cpos][0] == 0) {
            play_graph[prev_mpos][cpos][0] = CAT;
            q.emplace(prev_mpos, cpos, MOUSE);
          }
        }
      }
    }
  }

  return play_graph[1][2][0];
}
