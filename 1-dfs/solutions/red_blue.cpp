// Даны N точек, занумерованных числами 1, 2, ..., N.
// От каждой точки с меньшим номером к каждой точке с большим номером
// ведет стрелка красного или синего цвета.
// Раскраска стрелок называется однотонной, если нет двух таких точек A и B,
// что от A до B можно добраться как только по красным стрелкам, так и только по синим.
// Ваша задача — по заданной раскраске определить, является ли она однотонной. N до 5000.

#include <iostream>
#include <string>
#include <vector>
#include <set>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<std::set<uint32_t>> graph;
    graph.resize(5000);

    uint32_t N;
    std::cin >> N;

    std::string line;
    uint32_t cur = 0;
    while (cur + 1 < N && std::cin >> line) {
        for (uint32_t j = 0; j < line.size(); ++j)
            if (line[j] == 'R') {
                // идея: инвертируем красные ребра
                graph[cur + j + 1].insert(cur);
            } else {
                graph[cur].insert(cur + j + 1);
            }
        ++cur;
    }

    // идея: если в графе с инвертированными красными ребрами есть цикл,
    // то он не однотонный
    bool has_loops = false;
    uint8_t visited[5000]{};

    auto dfs = [&graph, &has_loops, &visited, N](this auto& self, uint32_t v) -> void {
        visited[v] = 1;

        for (uint32_t to = 0; to < N; ++to) {
            if (to == v || !graph[v].contains(to))
                continue;
            if (visited[to] == 1)
                has_loops = true;
            else if (graph[v].contains(to))
                self(to);
        }

        visited[v] = 2;
    };

    for (uint64_t i = 0; i < N; ++i)
        if (!has_loops && !visited[i])
            dfs(i);

    std::cout << (has_loops ? "NO" : "YES") << std::endl;
}
