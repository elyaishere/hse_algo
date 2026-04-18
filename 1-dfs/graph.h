#pragma once
#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

struct AdjMatrix {
  using Vertex = std::uint64_t;
  using value_type = bool;

  static bool is_valid(value_type v) noexcept { return v; }
};

struct AdjList {
  using Vertex = std::uint64_t;
  using value_type = Vertex;

  static bool is_valid(value_type) noexcept { return true; }
};

template <typename Rep> class Graph {
  class Edges {
    friend class iterator;

  public:
    using value_type = Rep::value_type;
    class iterator {
      using value_type = Edges::value_type;
      using inner_type = typename std::vector<value_type>::iterator;

    public:
      iterator(size_t pos, size_t size, const Edges &edges)
          : __pos(pos), __size(size), __edges(edges) {
        adjust();
      }

      bool operator!=(const iterator &other) { return __pos != other.__pos; }

      iterator &operator++() {
        ++__pos;
        adjust();
        return *this;
      }

      const auto &operator*() const {
        if constexpr (std::is_same_v<Rep, AdjList>) {
          return __edges.__data[__pos];
        } else {
          static_assert(std::is_same_v<Rep, AdjMatrix>);
          return __pos;
        }
      }

    private:
      size_t __pos, __size;
      const Edges &__edges;

      void adjust() {
        while (__pos != __size)
          if (Rep::is_valid(__edges.__data[__pos]))
            break;
          else
            ++__pos;
      }
    };

    iterator begin() const { return iterator(0, __data.size(), *this); }

    iterator end() const {
      return iterator(__data.size(), __data.size(), *this);
    }

    void resize(size_t size) { __data.resize(size, 0); }

    auto size() const noexcept { return __data.size(); }

    void set(Rep::Vertex v) {
      if constexpr (std::is_same_v<Rep, AdjList>) {
        __data.push_back(v);
      } else {
        static_assert(std::is_same_v<Rep, AdjMatrix>);
        __data[v] = true;
      }
    }

    void reset(Rep::Vertex v) {
      if constexpr (std::is_same_v<Rep, AdjList>) {
        auto it = std::ranges::find(__data, v);
        __data.erase(it);
      } else {
        static_assert(std::is_same_v<Rep, AdjMatrix>);
        if (v < __data.size())
          __data[v] = false;
      }
    }

  private:
    std::vector<value_type> __data;
  };

public:
  using Vertex = Rep::Vertex;
  using Edge = std::pair<Vertex, Vertex>;
  enum class Type { Indirected, Directed };

  static Graph create_indirected_graph(std::size_t size) {
    Graph g(size, Type::Indirected);
    return g;
  }

  static Graph create_indirect_graph(const std::vector<Edge> &edges) {
    Graph g(edges, Type::Indirected);
    return g;
  }

  static Graph create_directed_graph(std::size_t size) {
    Graph g(size, Type::Directed);
    return g;
  }

  static Graph create_directed_graph(const std::vector<Edge> &edges) {
    Graph g(edges, Type::Directed);
    return g;
  }

  const auto &edges(Vertex v) const { return __data[v]; }

  auto empty() const noexcept { return __data.empty(); }

  auto size() const noexcept { return __data.size(); }

  template <typename F> std::vector<Edge> traverse_dfs(F &&on_loop) const {
    std::vector<uint8_t> visited(__data.size(), 0);
    std::vector<Edge> tree;
    tree.reserve(__data.size());
    for (Vertex v = 0; v < __data.size(); ++v) {
      if (!visited[v])
        _traverse_dfs(visited, v, -1, tree, on_loop);
    }

    return tree;
  }

  Graph invert() const {
    Graph inverted(__data.size(), __type);

    for (Vertex from = 0; from < __data.size(); ++from) {
      for (auto to : edges(from)) {
        inverted.add_edge(to, from);
      }
    }

    return inverted;
  }

  void add_edge(const Edge &edge) { add_edge(edge.first, edge.second); }

  void add_edge(Vertex from, Vertex to) {
    _add_edge(from, to);
    if (__type == Type::Indirected) {
      _add_edge(to, from);
    }
  }

  void remove_edge(const Edge &edge) { remove_edge(edge.first, edge.second); }

  void remove_edge(Vertex from, Vertex to) {
    _remove_edge(from, to);
    if (__type == Type::Indirected) {
      _remove_edge(to, from);
    }
  }

  void set_type(Type tp) { __type = tp; }

private:
  Type __type;
  std::vector<Edges> __data;

  explicit Graph(std::size_t size, Type tp) : __data(size), __type(tp) {
    if constexpr (std::is_same_v<Rep, AdjMatrix>) {
      for (auto &e : __data)
        e.resize(size);
    }
  }

  explicit Graph(const std::vector<Edge> &edges, Type tp) : __type(tp) {
    auto size =
        std::reduce(edges.begin(), edges.end(), 0, [](auto a, const auto &b) {
          return std::max(a, std::max(b.first, b.second));
        });

    __data.resize(size);

    std::ranges::for_each(edges, [this](const auto &edge) { add_edge(edge); });
  }

  template <typename F>
  void _traverse_dfs(std::vector<uint8_t> &visited, Vertex v, Vertex p,
                     std::vector<Edge> &tree, F &&on_loop) const {
    visited[v] = 1;

    for (auto to : edges(v)) {
      if (!visited[to]) {
        tree.emplace_back(v, to);
        _traverse_dfs(visited, to, v, tree, on_loop);
      } else if (to != p && visited[to] == 1) { // loop
        on_loop(v, to);
      }
    }
    visited[v] = 2;
  }

  void _add_edge(Vertex from, Vertex to) { __data[from].set(to); }

  void _remove_edge(Vertex from, Vertex to) { __data[from].reset(to); }
};
