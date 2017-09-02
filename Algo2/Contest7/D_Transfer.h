#include <vector>
#include <algorithm>
#include <map>
#include <iostream>

const int64_t INF = 1000000007;

int32_t ReadInt32() {
  int32_t number;
  std::cin >> number;
  return number;
}

int64_t ReadInt64() {
  int64_t number;
  std::cin >> number;
  return number;
}

template<typename T>
void PrintVector(const std::vector<T>& some_vector) {
  for (const auto& element : some_vector) {
    std::cout << element << " ";
  }
  std::cout << "\n";
}

void PrintResult(const int64_t& result) {
  std::cout << result << "\n";
}

class Graph {
public:
  explicit Graph(int32_t vertices, int32_t edge_count) {
    vertices_ = vertices;
    edge_count_ = 0;
    edges_.resize(2 * edge_count);
  }

  void Reset(int32_t src, int32_t dest) {
    src_ = src;
    dest_ = dest;
    head_.assign(vertices_, -1);
    ques_.assign(vertices_, 0);
  }

  void AddEdge(int32_t f_vertex, int32_t s_vertex, int64_t cap, int64_t rc) {
    edges_[edge_count_] = edge(s_vertex, cap, 0, head_[f_vertex]);
    head_[f_vertex] = edge_count_++;
    edges_[edge_count_] = edge(f_vertex, rc, 0, head_[s_vertex]);
    head_[s_vertex] = edge_count_++;
  }

  int64_t MaxFlow() {
    int64_t ret = 0;
    while (_bfs()) {
      work_ = head_;
      bool flag = true;
      while (flag) {
        int64_t delta = _dfs(src_, INF);
        if (delta == 0) {
          break;
        }
        ret += delta;
      }
    }
    return ret;
  }

private:
  struct edge {
    int64_t vertex, cap, flow, nxt;
    edge() {}
    edge(int64_t vertex, int64_t cap, int64_t flow, int64_t nxt)
      : vertex(vertex), cap(cap), flow(flow), nxt(nxt) {}
  };

  bool _bfs() {
    level_.assign(vertices_, -1);
    int32_t le = 0;
    int32_t ri = 0;
    ques_[ri++] = src_;
    level_[src_] = 0;
    while (le < ri) {
      for (int32_t top = ques_[le++], i = head_[top]; i != -1; i = edges_[i].nxt) {
        if (edges_[i].flow < edges_[i].cap && level_[edges_[i].vertex] == -1) {
          level_[edges_[i].vertex] = level_[top] + 1;
          ques_[ri++] = edges_[i].vertex;
        }
      }
    }
    return (level_[dest_] != -1);
  }

  int64_t _dfs(int32_t vertex, int64_t flow) {
    if (vertex == dest_) {
      return flow;
    }

    for (int32_t& i = work_[vertex]; i != -1; i = edges_[i].nxt) {
      if (edges_[i].flow < edges_[i].cap && level_[vertex] + 1 == level_[edges_[i].vertex]) {
        int minf = _dfs(edges_[i].vertex, std::min(flow, edges_[i].cap - edges_[i].flow));
        if (minf > 0) {
          edges_[i].flow += minf;
          edges_[i ^ 1].flow -= minf;
          return minf;
        }
      }
    }

    return 0;
  }

  int32_t vertices_;
  int32_t edge_count_;
  std::vector<int32_t> head_;
  std::vector<int32_t> level_;
  int32_t src_;
  int32_t dest_;
  std::vector<int32_t> work_;
  std::vector<edge> edges_;
  std::vector<int32_t> ques_;
};

Graph ReadInstance(const int32_t vertice,
                   const int32_t edges_count,
                   const std::vector<int32_t>& weights) {
  std::map<std::pair<int32_t, int32_t>, std::pair<int32_t, int32_t>> edges;
  for (int i = 0; i < edges_count; ++i) {
    int32_t f_vertex = ReadInt32();
    int32_t s_vertex = ReadInt32();
    if (f_vertex < s_vertex) {
      edges[std::make_pair(f_vertex, s_vertex)].first = 100000000;
    } else {
      edges[std::make_pair(s_vertex, f_vertex)].second = 100000000;
    }
  }
  // std::cerr << "mda\n";
  Graph graph(vertice + 2, 2 * vertice + edges.size());
  graph.Reset(0, vertice + 1);

  for (const auto& element : edges) {
    int32_t f_vertex = element.first.first;
    int32_t s_vertex = element.first.second;
    int32_t cap = element.second.first;
    int32_t rcap = element.second.second;
    // std::cerr << f_vertex << "->" << s_vertex << " costs " << cap << ", " << rcap << "\n";
    graph.AddEdge(f_vertex, s_vertex, cap, rcap);
  }

  // std::cerr << "end\n";

  for (int i = 0; i < weights.size(); ++i) {
    graph.AddEdge(0, i + 1, weights[i], 0);
  }

  return graph;
}

std::vector<int32_t> ReadWeights(const int32_t weight_count) {
  std::vector<int32_t> weights(weight_count);
  for (int i = 0; i < weight_count; ++i) {
    weights[i] = ReadInt32();
  }
  return weights;
}

int64_t Appr(const Graph& graph, const int32_t& vertices, const std::vector<int32_t>& weights) {
  int32_t left = 0;
  int32_t right = weights[0];
  for (int i = 0; i < weights.size(); ++i) {
    right = std::max(right, weights[i]);
  }
  int64_t sum_weigths = std::accumulate(weights.begin(), weights.end(), 0);
  while (right - left > 1) {
    // std::cerr << left << " - " << right << "\n";
    int32_t middle = (left + right) / 2;
    Graph graph_copy = graph;
    for (int i = 0; i < vertices; ++i) {
      graph_copy.AddEdge(i + 1, vertices + 1, middle, 0);
    }
    int64_t flow = graph_copy.MaxFlow();
    if (flow == sum_weigths) {
      right = middle;
    } else {
      left = middle;
    }
  }
  return right;
}

int main() {

  int32_t vertice = ReadInt32();
  int32_t edges = ReadInt32();
  std::vector<int32_t> weights = ReadWeights(vertice);
  // PrintVector(weights);
  Graph instanse = ReadInstance(vertice, edges, weights);
  // std::cerr << "heh\n";
  int64_t result = Appr(instanse, vertice, weights);
  PrintResult(result);
}
