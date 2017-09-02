#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

int32_t ReadInt32() {
  int32_t number;
  std::cin >> number;
  return number;
}

void PrintResult(const int32_t& result) {
  if (result > 0) {
    std::cout << "YES\n" << result << "\n";
  } else {
    std::cout << "NO\n";
  }
}

template<typename T>
void PrintVector(const std::vector<T>& someVector) {
  for (auto& element : someVector) {
    std::cout << element << " ";
  }
  std::cout << "\n";
}

struct Edge{
  int32_t f_vertex;
  int32_t s_vertex;
  int32_t cost;

  friend std::ostream& operator<<(std::ostream& os, const Edge& edge) {
    os << edge.f_vertex << "->" << edge.s_vertex << ", cost : " << edge.cost << "\n";
    return os;
  }
};

std::vector<Edge> ReadEdges(const int32_t edge_count) {
  std::vector<Edge> edges;
  edges.reserve(edge_count);
  for (int i = 0; i < edge_count; ++i) {
    int32_t f_vertex = ReadInt32() - 1;
    int32_t s_vertex = ReadInt32() - 1;
    int32_t cost = ReadInt32();
    edges.push_back({f_vertex, s_vertex, cost});
  }
  return edges;
}




class Graph {
public:
  explicit Graph(const int32_t vertex_count) : vertex_count_(vertex_count) {
    graph_.resize(vertex_count_);
  }

  void AddEdge(const int32_t f_vertex, const int32_t s_vertex) {
    graph_[f_vertex].push_back(s_vertex);
  }

  const std::vector<int32_t>& GetNeighbours(const int32_t vertex) const {
    return graph_[vertex];
  }

  Graph Reversed() const {
    Graph reversed_graph(vertex_count_);
    for (int i = 0; i < graph_.size(); ++i) {
      for (int to : graph_[i]) {
        reversed_graph.AddEdge(to, i);
      }
    }
    return reversed_graph;
  }

  int32_t Size() const {
    return graph_.size();
  }

private:
  std::vector<std::vector<int32_t>> graph_;
  int32_t vertex_count_;
};


Graph Reduce(const int32_t vertex_count, const std::vector<Edge>& edges) {
  Graph graph(vertex_count);
  for (int i = 0; i < edges.size(); ++i) {
    graph.AddEdge(edges[i].f_vertex, edges[i].s_vertex);
  }
  return graph;
}

void dfs1 (const Graph& graph,
           int vertex,
           std::vector<bool>& used,
           std::vector<int32_t>& order) {
  used[vertex] = true;
  for (size_t i = 0; i < graph.GetNeighbours(vertex).size(); ++i) {
    if (!used[graph.GetNeighbours(vertex)[i]]) {
      dfs1(graph, graph.GetNeighbours(vertex)[i], used, order);
    }
  }
  order.push_back (vertex);
}

void dfs2 (const Graph& rgraph,
           int32_t vertex,
           std::vector<bool>& used,
           std::vector<int32_t>& order,
           std::vector<int32_t>& component) {
  used[vertex] = true;
  component.push_back(vertex);
  for (size_t i = 0; i < rgraph.GetNeighbours(vertex).size(); ++i) {
    if (!used[rgraph.GetNeighbours(vertex)[i]]) {
      dfs2(rgraph, rgraph.GetNeighbours(vertex)[i], used, order, component);
    }
  }
}

std::vector<int32_t> Components(const Graph& graph) {

  std::vector<int32_t> colors(graph.Size(), -1);

  int32_t size = graph.Size();
  std::vector<bool> used(size, false);
  std::vector<int32_t> order;
  std::vector<int32_t> component;

  for (int i = 0; i < size; ++i) {
    if (!used[i]) {
      dfs1(graph, i, used, order);
    }
  }
  used.assign (size, false);
  Graph rgraph = graph.Reversed();
  int32_t current_color = -1;
  for (int i = 0; i < size; ++i) {
    int vertex = order[size - 1 - i];
    if (!used[vertex]) {
      dfs2 (rgraph, vertex, used, order, component);
      ++current_color;
      for (int32_t vertex : component) {
        colors[vertex] = current_color;
      }
      component.clear();
    }
  }
  return colors;
}


class WeightedGraph {
public:
  explicit WeightedGraph(const int32_t vertex_count) : vertex_count_(vertex_count) {
    graph_.resize(vertex_count_);
  }

  void AddEdge(const int32_t f_vertex, const int32_t s_vertex, const int32_t cost) {
    graph_[f_vertex].push_back({f_vertex, s_vertex, cost});
  }

  const std::vector<Edge>& GetNeighbours(const int32_t vertex) const {
    return graph_[vertex];
  }

  int32_t Size() const {
    return graph_.size();
  }

  void SetMinimalOldVertex(int32_t old_label) {
    minimal_old_vertex = old_label;
  }

  int32_t GetMinimalOldVertex() const {
    return minimal_old_vertex;
  }

  std::vector<Edge> GetEdges() const {
    std::vector<Edge> edges;
    for (const auto& sub_edges : graph_) {
      for (const Edge& edge : sub_edges) {
        edges.push_back(edge);
      }
    }
    return edges;
  }

  void Print() const {
    // std::cout << "size = " << graph_.size() << "\n";
    // std::cout << "minimal = " << minimal_old_vertex << "\n";
    for (const auto& subv : graph_) {
      PrintVector(subv);
    }
  }

private:
  std::vector<std::vector<Edge>> graph_;
  int32_t vertex_count_;
  int32_t minimal_old_vertex;
};


WeightedGraph ConstructWeightedGraph(const std::vector<Edge>& edges) {
  std::set<int32_t> labels;
  for (const Edge& edge : edges) {
    labels.insert(edge.f_vertex);
    labels.insert(edge.s_vertex);
  }
  std::map<int32_t, int32_t> relabeling;
  int32_t size = 0;
  for (const auto it: labels) {

    relabeling[it] = size;
    ++size;
  }

  WeightedGraph graph(static_cast<int32_t>(labels.size()));

  for (const Edge& edge : edges) {
    graph.AddEdge(relabeling[edge.f_vertex], relabeling[edge.s_vertex], edge.cost);
  }

  graph.SetMinimalOldVertex(*labels.begin());

  return graph;
}

std::vector<WeightedGraph> ComponentGraphs(const int32_t vertex_count,
                                           const std::vector<Edge>& edges,
                                           const std::vector<int32_t>& colors) {
  std::vector<WeightedGraph> graphs;
  int32_t max_color = *std::max_element(colors.begin(), colors.end());
  // std::cerr << "max_color = " << max_color << "\n";
  std::vector<int32_t> color_counter(max_color + 1);
  for (int32_t color : colors) {
    ++color_counter[color];
  }
  // PrintVector(colors);
  // PrintVector(color_counter);

  std::vector<std::vector<Edge>> sub_edges(max_color + 1);

  for (const auto& edge : edges) {
    // std::cerr << edge.f_vertex << " " << edge.s_vertex << "\n";
    if (colors[edge.f_vertex] == colors[edge.s_vertex]) {
      int32_t current_color = colors[edge.f_vertex];
      sub_edges[current_color].push_back(edge);
    }
  }

  // std::cerr << "subs\n";
  // for (const auto& sub_edge : sub_edges) {
  //   PrintVector(sub_edge);
  // }

  for (int i = 0; i <= max_color; ++i) {
    graphs.push_back(ConstructWeightedGraph(sub_edges[i]));
  }

  return graphs;
}

bool FordBellman(const WeightedGraph& graph) {
  int32_t size = graph.Size();
  if (size == 0) {
    return false;
  }
  std::vector<Edge> edges = graph.GetEdges();
  std::vector<int32_t> distances (size, 100000000);
  distances[0] = 0;
  for (int iteration = 0; iteration < size + 1; ++iteration) {
    bool any = false;
    for (int j = 0; j < edges.size(); ++j) {
      if (distances[edges[j].f_vertex] < 100000000) {
        if (distances[edges[j].s_vertex] > distances[edges[j].f_vertex] + edges[j].cost) {
          distances[edges[j].s_vertex] = distances[edges[j].f_vertex] + edges[j].cost;
          any = true;
        }
      }
    }
    if (!any)  break;
    if (iteration == size) {
      return true;
    }
  }
  // вывод d, например, на экран
  return false;
}

int32_t GetResult(const int32_t vertex_count,
                  const std::vector<Edge>& edges,
                  const std::vector<int32_t>& colors) {
  std::vector<WeightedGraph> graphs = ComponentGraphs(vertex_count, edges, colors);
  // std::cerr << "phah\n";
  // for (const auto& graph : graphs) {
  //   graph.Print();
  // }
  std::sort(graphs.begin(), graphs.end(),
            [](const WeightedGraph & a, const WeightedGraph & b) -> bool {
                   return a.GetMinimalOldVertex() < b.GetMinimalOldVertex();
                 });
  for (const auto& graph : graphs) {
    if (FordBellman(graph)) {
      return graph.GetMinimalOldVertex() + 1;
    }
  }

  return -1;
}

int main() {
  int vertex_count = ReadInt32();
  int edge_count = ReadInt32();
  std::vector<Edge> edges = ReadEdges(edge_count);
  // std::cerr << "ffuuu\n";
  Graph graph = Reduce(vertex_count, edges);
  // std::cerr << "heh\n";
  std::vector<int32_t> colors = Components(graph);
  // PrintVector(colors);
  int32_t result = GetResult(vertex_count, edges, colors);
  PrintResult(result);
  return 0;
}
