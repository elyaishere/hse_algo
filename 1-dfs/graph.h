#pragma once

#include <vector>
#include <iostream>
#include <deque>

// неориентированный
class Graph {
    using Vertex = uint64_t;
    using Depth = uint64_t;
    using Matrix = std::vector<std::vector<uint8_t>>;
    using Row = std::vector<uint8_t>;

private:
    static void _dfs_erase(Matrix& gr, Vertex v) {
        for (Vertex to = 0; to < gr.size(); ++to)
            if (to != v && gr[v][to]) {
                gr[v][to] = gr[to][v] = 0;
                _dfs_erase(gr, to);
            }
        std::cout << v << ' ';
    }

    template <typename Func>
    static void _dfs(const Matrix& gr, Row& visited, std::vector<Depth>& depths, Vertex cur,
                     Func&& loop_callback, Vertex parent = -1, Depth d = 0) {
        // в случае ориентированного графа нужно 3 состояния
        visited[cur] = true;
        depths[cur] = d;

        for (Vertex to = 0; to < visited.size(); ++to) {
            if (to == cur || !gr[cur][to] || to == parent)
                continue;
            if (visited[to])
                loop_callback(cur, to);
            else
                _dfs(gr, visited, depths, to, std::forward<Func>(loop_callback), cur, d + 1);
        }
    }

public:
    explicit Graph(size_t size)
        : /* adjacency matrix (may be suboptimal) */
          adj_matrix_(size, Row(size, 0)) {
        vertex_depths_.resize(size, 0);
    }

    auto Size() const noexcept {
        return adj_matrix_.size();
    }

    void PrintEulerCycle() const {
        auto matrix = adj_matrix_;
        _dfs_erase(matrix, 0);

        /* alternatively
            auto dfs = [&matrix] (this auto& self, size_t v) -> void {
                for (size_t to = 0; to < matrix.size(); ++to)
                    if (matrix[v][to])
                    {
                        matrix[v][to] = matrix[to][v] = 0;
                        self(to);
                    }
                std::cout << v << ' ';
            };

            dfs(0);
        */

        std::cout << std::endl;
    }

    void FillVertexDepth() {
        std::fill(vertex_depths_.begin(), vertex_depths_.end(), 0);
        Row visited(adj_matrix_.size(), 0);

        for (uint64_t v = 0; v < visited.size(); ++v)
            if (!visited[v])
                _dfs(adj_matrix_, visited, vertex_depths_, v, /* nop */ [](Vertex, Vertex) {});
    }

    uint8_t& operator[](size_t i, size_t j) {
        return adj_matrix_[i][j];
    }

    const uint8_t& operator[](size_t i, size_t j) const {
        return adj_matrix_[i][j];
    }

    void PrintTree(bool with_depth = false) const {
        if (adj_matrix_.empty())
            return;

        struct decorated {
            size_t v;
            std::string prefix;
            bool is_last;
        };

        std::deque<decorated> vertices;
        vertices.emplace_back(0, "", true);

        while (!vertices.empty()) {
            auto [v, prefix, is_last] = vertices.front();
            vertices.pop_front();

            if (v)
                std::cout << prefix << "|" << std::endl;
            std::cout << prefix;
            std::cout << (is_last ? "|___ " : "|--- ");
            std::cout << v;

            if (with_depth) {
                std::cout << "[" << vertex_depths_[v] << "]";
            }

            std::cout << std::endl;

            prefix += (is_last ? "    " : "|    ");
            bool last = true;
            for (auto to = std::ssize(adj_matrix_) - 1; to > v; --to) {
                if (to == v || !adj_matrix_[v][to])
                    continue;

                if (last) {
                    vertices.emplace_front(to, prefix, true);

                    last = false;
                } else {
                    vertices.emplace_front(to, prefix, false);
                }
            }
        }
    }

    void RemoveLoops()
    // для неориентированного графа
    {
        std::fill(vertex_depths_.begin(), vertex_depths_.end(), 0);
        Row visited(adj_matrix_.size(), 0);
        std::vector<std::pair<Vertex, Vertex>> edges_to_remove;

        auto update_remove = [this, &edges_to_remove](Vertex from, Vertex to) {
            edges_to_remove.emplace_back(from, to);
        };

        for (Vertex v = 0; v < visited.size(); ++v)
            if (!visited[v])
                _dfs(adj_matrix_, visited, vertex_depths_, v, update_remove);

        for (const auto& edge : edges_to_remove) {
            auto [v1, v2] = edge;
            adj_matrix_[v1][v2] = adj_matrix_[v2][v1] = 0;
        }
    }

private:
    Matrix adj_matrix_;
    std::vector<uint64_t> vertex_depths_;
};
