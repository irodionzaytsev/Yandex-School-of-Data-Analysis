#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>


const int64_t MAXN = 105;

const int64_t INF = 1000000007;

using namespace std;

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
      while (true) {
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
    if(vertex == dest_) {
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


int main() {

  // std::cerr << "heh \n";
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  int32_t N = ReadInt32();
  int32_t K = ReadInt32();

  Graph graph(N, K);
  graph.Reset(0, N - 1);

  for (int i = 0; i < K; ++i) {
    int32_t a = ReadInt32() - 1;
    int32_t b = ReadInt32() - 1;
    int64_t c = ReadInt64();
    graph.AddEdge(a, b, c, c);
  }

  std::cout << graph.MaxFlow() << "\n";
  return 0;
}