#include <string>
#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>

void PrintResult(int32_t result);

int32_t ReadInt32();

enum Result {first = 1, second = 2, draw = 3};

Result ReadResult();

struct Game {
  int32_t first_participant;
  int32_t second_participant;
  Result result;
};

std::vector<Game> ReadGames(const int32_t games_count);

class Graph {
public:
  explicit Graph(const int32_t vertex_count, const std::vector<Game>& games, bool reverse = false);

  const std::vector<int32_t>& GetNeighbors(const int32_t vertex) const;

  size_t Size() const;

private:
  int32_t vertex_count_;
  std::vector<std::vector<int32_t>> graph_;
};

template <class ProcessStartVertex, class ProcessFinishVertex, class CheckUsedVertex>
void DepthFirstSearch(const Graph& graph,
                      const int32_t vertex,
                      ProcessStartVertex StartVertex,
                      ProcessFinishVertex FinishVertex,
                      CheckUsedVertex UsedVertex) {
  StartVertex(vertex);
  for (size_t index = 0; index < graph.GetNeighbors(vertex).size(); ++index) {
    if (!UsedVertex(graph.GetNeighbors(vertex)[index])) {
      DepthFirstSearch(graph,
                       graph.GetNeighbors(vertex)[index],
                       StartVertex,
                       FinishVertex,
                       UsedVertex);
    }
  }
  FinishVertex(vertex);
};


template <class ProcessStartVertex, class ProcessFinishVertex, class CheckUsedVertex>
void DepthFirstSearch(const Graph& graph,
                      const int32_t vertex,
                      ProcessStartVertex StartVertex,
                      ProcessFinishVertex FinishVertex,
                      CheckUsedVertex UsedVertex);

struct ComponentsInfo {
  std::vector<int32_t> components_sizes;
  std::vector<bool> clever_components;
};

int32_t MaximalAmountOfEmployees(const int32_t people_count, const std::vector<Game>& games);

int main() {
  int32_t people_count = ReadInt32();
  int32_t game_count = ReadInt32();
  std::vector<Game> games = ReadGames(game_count);
  int32_t result = MaximalAmountOfEmployees(people_count, games);
  PrintResult(result);

  return 0;
}

void PrintResult(int32_t result) {
  std::cout << result << "\n";
}

int32_t ReadInt32() {
  int32_t num;
  std::cin >> num;
  return num;
}

Result ReadResult() {
  int32_t num;
  std::cin >> num;
  if (num == 1) {
    return first;
  }
  if (num == 2) {
    return second;
  }
  return draw;
}

std::vector<Game> ReadGames(const int32_t games_count) {
  std::vector<Game> games;
  games.reserve(games_count);
  for (int game_index = 0; game_index < games_count; ++game_index) {
    int32_t first_participant = ReadInt32();
    int32_t second_participant = ReadInt32();
    Result result = ReadResult();
    games.push_back({first_participant, second_participant, result});
  }
  return games;
}

Graph::Graph(const int32_t vertex_count, const std::vector<Game>& games, bool reverse) {
    vertex_count_ = vertex_count;
    graph_.resize(vertex_count_);
    for (const Game& game : games) {
      if ((game.result == first && !reverse) || (game.result == second && reverse)) {
        graph_[game.first_participant - 1].push_back(game.second_participant - 1);
      } else if ((game.result == second && !reverse) || (game.result == first && reverse)) {
        graph_[game.second_participant - 1].push_back(game.first_participant - 1);
      }
    }
  }

  const std::vector<int32_t>& Graph::GetNeighbors(const int32_t vertex) const {
    return graph_[vertex];
  }

  size_t Graph::Size() const {
    return graph_.size();
  }

ComponentsInfo GetComponentsInfo(const Graph &graph, const Graph &reversed_graph) {

  int32_t people_count = graph.Size();

  std::vector<bool> used(people_count, false);
  std::vector<int32_t> order;

  for (int vertex = 0; vertex < people_count; ++vertex) {
    if (used[vertex]) {
      continue;
    }
    DepthFirstSearch(graph,
                     vertex,
                     [&used](const int32_t vertex) {
                       used[vertex] = true;
                     },
                     [&order](const int32_t vertex) {
                       order.push_back(vertex);
                     },
                     [&used](const int32_t vertex) {
                       return used[vertex];
                     });
  }

  used.assign(people_count, false);
  std::vector<int32_t> component_labels(people_count);
  std::vector<int32_t> component_sizes;
  int32_t current_component = -1;
  for (int32_t index = static_cast<int32_t>(order.size()) - 1; index >= 0; --index) {
    int32_t vertex = order[index];
    if (used[vertex]) {
      continue;
    }
    ++current_component;
    component_sizes.push_back(0);

    DepthFirstSearch(reversed_graph,
                     vertex,
                     [&used, &component_labels, &current_component, &component_sizes]
                       (const int32_t vertex) {
                       used[vertex] = true;
                       component_labels[vertex] = current_component;
                       ++component_sizes.back();
                     },
                     [](const int32_t vertex) {},
                     [&used](const int32_t vertex) {
                       return used[vertex];
                     });
  }

  std::vector<bool> clever_components(component_sizes.size(), true);

  for (int vertex = 0; vertex < people_count; ++vertex) {
    for (int to : graph.GetNeighbors(vertex)) {
      int32_t clever_component = component_labels[vertex];
      int32_t stupid_component = component_labels[to];
      if (clever_component != stupid_component) {
        clever_components[stupid_component] = false;
      }
    }
  }

  return ComponentsInfo{component_sizes, clever_components};
}

int32_t MaximalAmountOfEmployees(const int32_t people_count, const std::vector<Game>& games) {
  Graph graph(people_count, games);
  Graph reversed_graph(people_count, games, true);
  ComponentsInfo components_info = GetComponentsInfo(graph, reversed_graph);
  std::vector<int32_t> clever_component_sizes;
  for (int i = 0; i < components_info.clever_components.size(); ++i) {
    if (components_info.clever_components[i]) {
      clever_component_sizes.push_back(components_info.components_sizes[i]);
    }
  }

  return people_count + 1 -
    *std::min_element(clever_component_sizes.begin(), clever_component_sizes.end());
}


