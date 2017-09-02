#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

int32_t ReadInt32() {
  int32_t number;
  std::cin >> number;
  return number;
}

std::vector<int32_t> ReadVector(const int32_t elements) {
  std::vector<int32_t> result(elements);
  for (int i = 0; i < elements; ++i) {
    std::cin >> result[i];
  }
  return result;
}

template<typename T>
void PrintVector(const std::vector<T>& some_vector) {
  for (const auto& element : some_vector) {
    std::cout << element << " ";
  }
  std::cout << "\n";
}

struct Edge{
  int32_t s_vertex;
  int32_t d_vertex;

  friend std::ostream& operator<<(std::ostream& os, const Edge& edge) {
    os << "{" << edge.s_vertex << " -> " << edge.d_vertex << "} ";
    return os;
  }
};

std::vector<Edge> ReadEdges(const int32_t edges_count) {
  std::vector<Edge> edges;
  for (int i = 0; i < edges_count; ++i) {
    int32_t s_vertex = ReadInt32() - 1;
    int32_t d_vertex = ReadInt32() - 1;
    edges.push_back({s_vertex, d_vertex});
  }
  return edges;
}

void PrintResult(const int32_t result) {
  std::cout << result << "\n";
}


class DSU {

public:
  explicit DSU(int count) {
    parent.resize(count);
    rank.resize(count);
    for (int i = 0; i < count; ++i) {
      make_set(i);
    }
  }

  void make_set (int vertex) {
    parent[vertex] = vertex;
    rank[vertex] = 0;
  }

  int find_set (int vertex) {
    if (vertex == parent[vertex])
      return vertex;
    return parent[vertex] = find_set (parent[vertex]);
  }

  void union_sets (int element_a, int element_b) {
    element_a = find_set (element_a);
    element_b = find_set (element_b);
    if (element_a != element_b) {
      if (rank[element_a] < rank[element_b])
        std::swap (element_a, element_b);
      parent[element_b] = element_a;
      if (rank[element_a] == rank[element_b])
        ++rank[element_a];
    }
  }

private:
  std::vector<int> parent;
  std::vector<int> rank;
};


int32_t MaxFlow(const int32_t vertice_count, const std::vector<int32_t>& left_caps,
                const std::vector<int32_t>& right_caps, const std::vector<Edge>& edges) {
  DSU dsu(2 * vertice_count);
  for (const Edge& edge : edges) {
    dsu.union_sets(edge.s_vertex, vertice_count + edge.d_vertex);
  }

  // std::cerr << "lol\n";

  std::map<int32_t, int32_t> right_nominals;
  std::map<int32_t, int32_t> left_nominals;

  std::set<int32_t> colors;

  for (int i = 0; i < vertice_count; ++i) {
    // std::cerr << "i = " << i << ", set = " << dsu.find_set(i) << "\n";
    left_nominals[dsu.find_set(i)] += left_caps[i];
    colors.insert(dsu.find_set(i));
  }

  for (int i = vertice_count; i < 2 * vertice_count; ++i) {
    // std::cerr << "heh, i = " << i << ", set = " << dsu.find_set(i) << "\n";
    right_nominals[dsu.find_set(i)] += right_caps[i - vertice_count];
  }

  int32_t result = 0;
  for (int32_t color : colors) {
    result += std::min(left_nominals[color], right_nominals[color]);
  }

  return result;
}


int main() {
  int vertice_count = ReadInt32();
  int edges_count = ReadInt32();
  std::vector<int32_t> left_caps = ReadVector(vertice_count);
  // PrintVector(left_caps);
  std::vector<int32_t> right_caps = ReadVector(vertice_count);
  // PrintVector(right_caps);
  std::vector<Edge> edges = ReadEdges(edges_count);
  // PrintVector(edges);
  int32_t result = MaxFlow(vertice_count, left_caps, right_caps, edges);
  PrintResult(result);
}
