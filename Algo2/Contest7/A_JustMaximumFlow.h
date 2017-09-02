#include <iostream>
#include <vector>

int32_t ReadInt() {
  int number;
  std::cin >> number;
  return number;
}

struct Edge {
  int32_t first;
  int32_t second;
  int32_t cap;
  int32_t flow;
};

class Graph {
public:
  Graph(const int32_t vertex_count)
    : vertex_count_(vertex_count) {}

  void AddEdge(const int32_t fir, const int32_t sec, const int32_t cap) {
    Edge straight_edge = {fir, sec, cap, 0};
    Edge reversed_edge = {sec, fir, 0, 0};
    graph_links_[fir].push_back (edges.size());
    edges.push_back(straight_edge);
    graph_links_[sec].push_back(edges.size());
    edges.push_back(reversed_edge);
  }

private:
  int vertex_count_;
  std::vector<Edge> edges;
  std::vector<std::vector<int32_t>> graph_links_;
};

Graph ReadGraph(const int32_t vertice, const int32_t edges) {
  Graph graph(2 * vertice + 2);

  for (int i =  0; i < vertice; ++i) {
    int32_t flow = ReadInt();
    graph.AddEdge({})
  }

  for (int i = 0; i < K; ++i) {
    int first_vertex = ReadInt();
    int second_vertex = ReadInt();
  }
}

const int MAXN = ...; // число вершин
const int INF = 1000000000; // константа-бесконечность

struct edge {
  int a, b, cap, flow;
};

int n, s, t, d[MAXN], ptr[MAXN], q[MAXN];
vector<edge> e;
vector<int> g[MAXN];

void add_edge (int a, int b, int cap) {
  edge e1 = { a, b, cap, 0 };
  edge e2 = { b, a, 0, 0 };
  g[a].push_back ((int) e.size());
  e.push_back (e1);
  g[b].push_back ((int) e.size());
  e.push_back (e2);
}

bool bfs() {
  int qh=0, qt=0;
  q[qt++] = s;
  memset (d, -1, n * sizeof d[0]);
  d[s] = 0;
  while (qh < qt && d[t] == -1) {
    int v = q[qh++];
    for (size_t i=0; i<g[v].size(); ++i) {
      int id = g[v][i],
        to = e[id].b;
      if (d[to] == -1 && e[id].flow < e[id].cap) {
        q[qt++] = to;
        d[to] = d[v] + 1;
      }
    }
  }
  return d[t] != -1;
}

int dfs (int v, int flow) {
  if (!flow)  return 0;
  if (v == t)  return flow;
  for (; ptr[v]<(int)g[v].size(); ++ptr[v]) {
    int id = g[v][ptr[v]],
      to = e[id].b;
    if (d[to] != d[v] + 1)  continue;
    int pushed = dfs (to, min (flow, e[id].cap - e[id].flow));
    if (pushed) {
      e[id].flow += pushed;
      e[id^1].flow -= pushed;
      return pushed;
    }
  }
  return 0;
}

int32_t dinic() {
  int32_t flow = 0;
  for (;;) {
    if (!bfs())  break;
    memset (ptr, 0, n * sizeof ptr[0]);
    while (int pushed = dfs (s, INF))
      flow += pushed;
  }
  return flow;
}



int main() {
  int vertice = ReadInt();
  int edges = ReadInt();


}