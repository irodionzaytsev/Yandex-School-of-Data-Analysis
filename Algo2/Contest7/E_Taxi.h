#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <utility>
#include <string>

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

std::string ReadString() {
  std::string str;
  std::cin >> str;
  return str;
}

void PrintResult(const int32_t result) {
  std::cout << result << "\n";
}

class Graph {
public:
  explicit Graph(int64_t vertices, int32_t edge_count) {
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
          flag = false;
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

struct Call {
  int32_t minutes;
  int32_t start_x;
  int32_t start_y;
  int32_t finish_x;
  int32_t finish_y;
};

int32_t ToNumber(const char& cipher) {
  return cipher - '0';
}

int32_t ToMinutes(const std::string& str) {
  return 60 * (ToNumber(str[0]) * 10 + ToNumber(str[1])) + 10 * ToNumber(str[3]) + ToNumber(str[4]);
}

std::vector<Call> ReadCalls(const int32_t calls_count) {
  std::vector<Call> calls;
  calls.reserve(calls_count);
  for (int i = 0; i < calls_count; ++i) {
    std::string time = ReadString();
    int32_t start_x = ReadInt32();
    int32_t start_y = ReadInt32();
    int32_t finish_x = ReadInt32();
    int32_t finish_y = ReadInt32();
    calls.push_back({ToMinutes(time), start_x, start_y, finish_x, finish_y});
  }
  return calls;
}

struct Edge {
  int32_t src;
  int32_t dst;
};

int32_t Distance(const int32_t start_x, const int32_t start_y,
                 const int32_t finish_x, const int32_t finish_y) {
  return abs(finish_x - start_x) + abs(finish_y - start_y);
}

bool IsEdge(const Call& first_call, const Call& second_call) {
  int32_t f_dist = Distance(first_call.start_x, first_call.start_y,
                            first_call.finish_x, first_call.finish_y);
  int32_t s_dist = Distance(first_call.finish_x, first_call.finish_y,
                            second_call.start_x, second_call.start_y);
  return first_call.minutes + f_dist + s_dist < second_call.minutes;
}

std::vector<Edge> GetEdges(const int32_t calls_count, const std::vector<Call>& calls) {
  std::vector<Edge> edges;
  for (int32_t i = 0; i < calls_count; ++i) {
    for (int32_t j = i + 1; j < calls_count; ++j) {
      if (IsEdge(calls[i], calls[j])) {
        edges.push_back({i + 1, j + 1});
      }
    }
  }
  return edges;
}

int32_t Solve(const int32_t calls_count, const std::vector<Edge>& edges) {
  Graph graph(2 * calls_count + 2, edges.size() + 2 * calls_count);
  graph.Reset(0, 2 * calls_count + 1);
  for (int i = 1; i <= calls_count; ++i) {
    graph.AddEdge(0, i, 1, 0);
    graph.AddEdge(i + calls_count, 2 * calls_count + 1, 1, 0);
    // graph.AddEdge(i, calls_count + i, 0, 1);
  }

  for (const Edge& edge : edges) {
    graph.AddEdge(edge.src, calls_count + edge.dst, 1, 0);
  }

  return calls_count - graph.MaxFlow();
}


int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);

  int32_t calls_count = ReadInt32();
  std::vector<Call> calls = ReadCalls(calls_count);
  std::vector<Edge> edges = GetEdges(calls_count, calls);

  // std::cerr << "calls_count = " << calls_count << "\n";
  // std::cerr << "calls.size() = " << calls.size() << "\n";
  // std::cerr << "edges.size() = " << edges.size() << "\n";

  // std::cerr << "ready!\n";

  // for (const Edge& edge : edges) {
  //   std::cerr << edge.src << " " << edge.dst << "\n";
  // }

  int32_t result = Solve(calls_count, edges);
  PrintResult(result);

  return 0;
}
