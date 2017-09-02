#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

class Graph{
public:
  explicit Graph() {}

  explicit Graph(const int size) {
    graph_.resize(size);
  }

  void AddEdge(const int f_vertex, const int s_vertex, const int cost) {
    graph_[f_vertex].push_back(s_vertex);
    graph_[s_vertex].push_back(f_vertex);
    edge_price_[std::make_pair(f_vertex, s_vertex)] = cost;
    edge_price_[std::make_pair(s_vertex, f_vertex)] = cost;
  }

  int GetSize() const {
    return graph_.size();
  }

  std::vector<int> Neighbours(int vertex) const {
    return graph_[vertex];
  }

  int GetPrice(const int vertex, const int to) const {
    auto it = edge_price_.find(std::make_pair(vertex, to));
    return it->second;
  }

private:
  std::vector<std::vector<int>> graph_;
  std::map<std::pair<int, int>, int> edge_price_;
};

int ReadInt() {
  int number;
  std::cin >> number;
  return number;
}

Graph ReadGraph() {

  int size = ReadInt();
  int edges = ReadInt();

  Graph graph(size);

  for (int i = 0; i < edges; ++i) {
    int from = ReadInt() - 1;
    int to = ReadInt() - 1;
    int cost = ReadInt();
    graph.AddEdge(from, to, cost);
  }
  return graph;
}

struct GraphPackage {

  Graph graph;
  std::vector<bool> used;
  std::vector<int> fup;
  std::vector<int> tin;
  int timer;
};

template <class SHIT>
void dfs (int vertex,
          int parent,
          GraphPackage* graph_package,
          SHIT bridge_process) {

  // std::cout << "vertex, parent = " << vertex << ", " << parent << "\n";

  (graph_package->used)[vertex] = true;
  (graph_package->fup)[vertex] = (graph_package->timer)++;
  (graph_package->tin)[vertex] = (graph_package->fup)[vertex];

  for (size_t index = 0; index < graph_package->graph.Neighbours(vertex).size(); ++index) {
    // int to = g[v][i];
    int to = graph_package->graph.Neighbours(vertex)[index];
    // std::cout << "to = " << to << "\n";
    if (to == parent) {
      continue;
    }
    if ((graph_package->used)[to]) {
      (graph_package->fup)[vertex] =
        std::min((graph_package->fup)[vertex], (graph_package->tin)[to]);
    } else {
      dfs(to, vertex, graph_package, bridge_process);
      (graph_package->fup)[vertex] =
        std::min((graph_package->fup)[vertex], (graph_package->fup)[to]);
      if ((graph_package->fup)[to] > (graph_package->tin)[vertex]) {
        bridge_process(graph_package->graph.GetPrice(vertex, to));
      }
    }
  }
}

int CheapestEdge(GraphPackage* graph_package) {
  const int size = (graph_package->graph).GetSize();
  graph_package->used = std::vector<bool>(size, false);
  graph_package->timer = 0;
  graph_package->tin = std::vector<int>(size, 0);
  graph_package->fup = std::vector<int>(size, 0);

  int minima_cost = -1;

  for (int vertex = 0; vertex < size; ++vertex) {
    if (!graph_package->used[vertex]) {
      dfs(vertex, -1, graph_package,
          [&minima_cost](int cost) {
            if (minima_cost < 0 || minima_cost > cost) {
              minima_cost = cost;
            }
          });
    }
  }

  return minima_cost;
}

void PrintResult(const int result) {
  std::cout << result << "\n";
}

int main() {

  GraphPackage graph_package;
  graph_package.graph = ReadGraph();
  int result = CheapestEdge(&graph_package);
  PrintResult(result);

  return 0;
}
