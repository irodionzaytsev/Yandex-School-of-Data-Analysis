#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>
#include <map>
#include <utility>
#include <string>

const int32_t INF = 10000000;


std::map<char, int32_t> dir_codes = {{'N', 0}, {'W', 1}, {'S', 2}, {'E', 3}};
std::map<char, int32_t> rot_codes = {{'L', 0}, {'R', 1}};
std::map<std::string, std::vector<std::string>> movements = {
  {"NL", {"NL", "WR", "SL"}},
  {"NR", {"NR", "EL", "SR"}},

  {"WL", {"WL", "SR", "EL"}},
  {"WR", {"WR", "NL", "ER"}},

  {"SL", {"SL", "ER", "NL"}},
  {"SR", {"SR", "WL", "NR"}},

  {"EL", {"EL", "NR", "WL"}},
  {"ER", {"ER", "SL", "WR"}}
};

std::map<std::string, std::vector<std::string>> reversed_movements = {
  {"NL", {"NL", "WR", "SL"}},
  {"NR", {"NR", "SR", "EL"}},

  {"WL", {"WL", "SR", "EL"}},
  {"WR", {"NL", "WR", "ER"}},

  {"SL", {"NL", "SL", "ER"}},
  {"SR", {"NR", "WL", "SR"}},

  {"EL", {"NR", "WL", "EL"}},
  {"ER", {"WR", "SL", "ER"}}
};

std::map<char, std::pair<int32_t, int32_t>> shifts = {
  {'N', std::make_pair(-1, 0)},
  {'S', std::make_pair(1, 0)},
  {'W', std::make_pair(0, -1)},
  {'E', std::make_pair(0, 1)}
};

template<typename T>
void PrintVector(const std::vector<T>& some_vector) {
  for (const auto& element : some_vector) {
    std::cout << element << " ";
  }
  std::cout << "\n";
}

int32_t ReadInt32() {
  int32_t number;
  std::cin >> number;
  return number;
}

std::vector<std::string> ReadStrings(int32_t rows, int32_t columns) {
  std::vector<std::string> strings(rows);
  for (int i = 0; i < rows; ++i) {
    std::cin >> strings[i];
  }
  return strings;
}

struct Result{
  bool succes;
  std::vector<std::pair<int32_t, int32_t>> cell_coords;
};

void PrintResult(const Result& result) {
  if (!result.succes) {
    std::cout << "-1\n";
    return;
  }
  std::cout << result.cell_coords.size() - 1 << "\n";
  for (int i = 0; i < result.cell_coords.size(); ++i) {
    std::cout << result.cell_coords[i].second + 1 << " " << result.cell_coords[i].first + 1 << "\n";
  }
}


std::pair<int32_t, int32_t> DecodePair(int32_t code,
                                       const int32_t rows,
                                       const int32_t columns) {
  code /= 8;
  return std::make_pair(code % columns, code / columns);
};

struct Cell {
  int32_t rr;
  int32_t cc;
  char dir;
  char rot;
};


int32_t Code(int32_t rr, int32_t cc,
             char dir, char rot,
             int32_t rows, int32_t columns) {
  return rot_codes[rot] + 2 * dir_codes[dir] + 2 * 4 * cc + 2 * 4 * columns * rr;
}


Cell Decoder(int32_t code, const int32_t rows, const int32_t columns) {
  Cell cell;
  int32_t rot_code = code % 2;
  if (rot_code == 0) {
    cell.rot = 'L';
  } else {
    cell.rot = 'R';
  }
  code /= 2;
  int32_t dir_code = code % 4;
  if (dir_code == 0) {
    cell.dir = 'N';
  }
  if (dir_code == 1) {
    cell.dir = 'W';
  }
  if (dir_code == 2) {
    cell.dir = 'S';
  }
  if (dir_code == 3) {
    cell.dir = 'E';
  }
  code /= 4;
  cell.cc = code % columns;
  cell.rr = code / columns;
  return cell;
}


bool IsInside(int32_t rr, int32_t cc,
              int32_t rows, int32_t columns) {
  return (rr >= 0 && cc >= 0 && rr < rows && cc < columns);
}


std::vector<int32_t> GetNeighbours(const int32_t rr, const int32_t cc,
                                   const char dir, const char rot,
                                   const int32_t rows, const int32_t columns,
                                   const std::vector<std::string>& strings,
                                   std::map<std::string, std::vector<std::string>>& inner_moves) {
  std::vector<int32_t> neighbours;
  std::string key = {dir, rot};
  std::vector<std::string> moves = inner_moves[key];
  for (std::string move : moves) {
    char n_dir = move[0];
    char n_rot = move[1];
    std::pair<int32_t, int32_t> shift = shifts[n_dir];
    int32_t n_rr = rr + shift.first;
    int32_t n_cc = cc + shift.second;
    if (IsInside(n_rr, n_cc, rows, columns) && strings[n_rr][n_cc] != '#') {
      neighbours.push_back(Code(n_rr, n_cc,
                                n_dir, n_rot,
                                rows, columns));
    }
  }
  return neighbours;
}


class Grapher {
public:
  Grapher() {}

  Grapher(const int32_t rows, const int32_t columns, bool straight)
  : rows_(rows), columns_(columns) {
    if (straight) {
      inner_moves = movements;
    } else {
      inner_moves = reversed_movements;
    }
  }

  std::vector<int32_t> Neighbours(int32_t cell_code, const std::vector<std::string>& strings) {
    Cell cell = Decoder(cell_code, rows_, columns_);
    // std::cerr << "rr, cc, dir, rot = " << cell.rr << ", " << cell.cc << ", " << cell.dir << ", " << cell.rot << "\n";
    return GetNeighbours(cell.rr, cell.cc, cell.dir, cell.rot, rows_, columns_, strings, inner_moves);
  }

private:
  int32_t rows_;
  int32_t columns_;
  // std::vector<std::string>* strings_ptr_;
  std::map<std::string, std::vector<std::string>> inner_moves;
};


class Path {
public:
  Path() {}

  Path(const int32_t start_vertex,
       Grapher reversed_grapher,
       // std::vector<std::vector<int32_t>>* reversed_graph,
       const std::vector<int32_t>& distances)
    : start_vertex_(start_vertex), reversed_grapher_(reversed_grapher), distances_(distances) {}

  bool IsAvailable(int32_t finish_vertex) {
    return distances_[finish_vertex] < INF;
  }

  int32_t GetLength(int32_t finish_vertex) {
    std::cout << "distances = " << "\n";
    PrintVector(distances_);
    return distances_[finish_vertex];
  }

  std::vector<int32_t> GetPath(int32_t finish_vertex, const std::vector<std::string>& strings) {
    std::vector<int32_t> path;
    path.push_back(finish_vertex);
    while (path.back() != start_vertex_) {
      int32_t current_vertex = path.back();
      std::vector<int32_t> neighbours = reversed_grapher_.Neighbours(current_vertex, strings);
      for (int32_t to : neighbours) {
        if (distances_[to] + 1 == distances_[current_vertex]) {
          path.push_back(to);
          break;
        }
      }
    }
    return path;
  }

private:
  int32_t start_vertex_;
  Grapher reversed_grapher_;
  // std::vector<std::vector<int32_t>>* reversed_graph_;
  std::vector<int32_t> distances_;
};

class Graph {
public:
  Graph (int32_t rows, int32_t columns) {
    vertex_count_ = 2 * 4 * rows * columns;
    grapher_ = Grapher(rows, columns, true);
    reversed_grapher_ = Grapher(rows, columns, false);
    // graph_.resize(vertex_count);
    // reversed_graph_.resize(vertex_count);
  }

  /*
  void AddEdge(int32_t f_vertex, int32_t s_vertex) {
    graph_[f_vertex].push_back(s_vertex);
    reversed_graph_[s_vertex].push_back(f_vertex);
  }
   */

  Path GetBFSPaths(int32_t vertex, const std::vector<std::string>& strings) {
    // std::cerr << "vertex_count_ = " << vertex_count_ << "\n";
    std::queue<int32_t> vertices_to_process;
    std::vector<int32_t> distances(vertex_count_, INF);
    std::vector<bool> used(vertex_count_, false);
    vertices_to_process.push(vertex);
    distances[vertex] = 0;
    used[vertex] = true;
    while (!vertices_to_process.empty()) {
      // std::cerr << "here\n";
      int32_t current_vertex = vertices_to_process.front();
      vertices_to_process.pop();
      std::cerr << "vertex = " << current_vertex << "\n";
      std::vector<int32_t> neighbours = grapher_.Neighbours(current_vertex, strings);

      for (const int32_t to : neighbours) {
        std::cerr << "to = " << to << "\n";
        if (!used[to]) {
          used[to] = true;
          distances[to] = distances[current_vertex] + 1;
          vertices_to_process.push(to);
        }
      }
      // std::cerr << "succhiuhiuhiuhuhuhiuhuhies\n";
      // std::cerr << vertices_to_process.size() << "\n";
    }
    // for(int i = 0; i < 10000; ++i) {
    //   std::cerr << "phah\n";
    //   std::cerr << "return lol\n";
    // }
    return Path(vertex, reversed_grapher_, distances);
  }

private:
  int32_t vertex_count_;
  Grapher grapher_;
  Grapher reversed_grapher_;
  // std::vector<std::vector<int32_t>> reversed_graph_;
  // std::vector<std::vector<int32_t>> graph_;
};


/*
Graph ConstructGraph(const int32_t rows,
                     const int32_t columns,
                     std::vector<std::string>& strings) {

  Graph graph(rows, columns, &strings);

  for (int rr = 0; rr < rows; ++rr) {
    for (int cc = 0; cc < columns; ++cc) {
      for (char dir : {'N', 'W', 'S', 'E'}) {
        for (char rot : {'L', 'R'}) {
          int32_t cell_code = Code(rr, cc,
                                   dir, rot,
                                   rows, columns);
          std::vector<int32_t> neigbour_codes = GetNeighbours(rr, cc,
                                                              dir, rot,
                                                              rows, columns,
                                                              strings);
          for (const int32_t code : neigbour_codes) {
            graph.AddEdge(cell_code, code);
          }
        }
      }
    }
  }
  return graph;
}
 */

Result Solve(const int32_t rows, const int32_t columns,
                           std::vector<std::string>& strings,
                           const int32_t start_x, const int32_t start_y,
                           const int32_t finish_x, const int32_t finish_y) {
  std::cerr << "start\n";
  // Graph graph = ConstructGraph(rows, columns, strings);
  Graph graph(rows, columns);
  std::cerr << "did graph\n";
  int32_t minimal_path = 1000001;
  Path best_path;
  int32_t best_start_code = Code(start_x, start_y, 'N', 'L', rows, columns);
  int32_t best_finish_code = Code(finish_x, finish_y, 'N', 'L', rows, columns);
  for (char dir : {'N', 'W', 'S', 'E'}) {
    for (char rot : {'R', 'L'}) {
      int32_t current_start_code = Code(start_x, start_y, dir, rot, rows, columns);
      std::cerr << "dir = " << dir << ", rot = " << rot << "\n";
      Path paths = graph.GetBFSPaths(current_start_code, strings);
      std::cerr << "got paths\n";
      for (char f_dir : {'N', 'W', 'S', 'E'}) {
        for (char f_rot : {'R', 'L'}) {
          std::cerr << "f_dir = " << f_dir << ", f_rot = " << f_rot << "\n";
          int32_t current_finish_code = Code(finish_x, finish_y, f_dir, f_rot, rows, columns);
          std::cerr << "len = " << paths.GetLength(current_finish_code) << "\n";
          if (paths.GetLength(current_finish_code) < minimal_path) {
            minimal_path = paths.GetLength(current_finish_code);
            best_path = paths;
            best_start_code = current_start_code;
            best_finish_code = current_finish_code;
          }
        }
      }
    }
  }
  if (minimal_path < 1000001) {
    std::vector<std::pair<int32_t, int32_t>> cell_coords;
    std::vector<int32_t> best_codes = best_path.GetPath(best_finish_code, strings);
    std::reverse(best_codes.begin(), best_codes.end());
    std::vector<std::pair<int32_t, int32_t>> best_coords;
    for (const int32_t code : best_codes) {
      best_coords.push_back(DecodePair(code, rows, columns));
    }
    return {true, best_coords};
  } else {
    return {false, {}};
  }
}


int main() {
  int32_t rows = ReadInt32();
  int32_t columns = ReadInt32();
  std::vector<std::string> strings = ReadStrings(rows, columns);
  int32_t start_x  = ReadInt32() - 1;
  int32_t start_y = ReadInt32() - 1;
  int32_t finish_x = ReadInt32() - 1;
  int32_t finish_y = ReadInt32() - 1;
  Result result = Solve(rows, columns,
                        strings,
                        start_x, start_y,
                        finish_x, finish_y);
  PrintResult(result);
  return 0;
}
