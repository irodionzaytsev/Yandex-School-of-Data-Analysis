#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>
#include <map>
#include <utility>
#include <string>
#include <set>

const int32_t INF = 10000000;

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

std::vector<std::vector<bool>> ReadStrings(int32_t rows, int32_t columns) {
  std::vector<std::vector<bool>> strings(rows, std::vector<bool> (columns, true));
  for (int i = 0; i < rows; ++i) {
    std::string row;
    std::cin >> row;
    for (int j = 0; j < columns; ++j) {
      if (row[j] == '#') {
        strings[i][j] = false;
      }
    }
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

std::vector<std::vector<std::vector<std::pair<int8_t, int8_t>>>> int_movements = {
  {
    {{0, 0}, {1, 1}, {2, 0}},
    {{0, 1}, {3, 0}, {2, 1}}
  },
  {
    {{1, 0}, {2, 1}, {3, 0}},
    {{1, 1}, {0, 0}, {3, 1}}
  },
  {
    {{2, 0}, {3, 1}, {0, 0}},
    {{2, 1}, {1, 0}, {0, 1}}
  },
  {
    {{3, 0}, {0, 1}, {1, 0}},
    {{3, 1}, {2, 0}, {1, 1}}
  }
};

std::map<char, std::pair<int8_t, int8_t>> shifts = {
  {'N', std::make_pair(-1, 0)},
  {'S', std::make_pair(1, 0)},
  {'W', std::make_pair(0, -1)},
  {'E', std::make_pair(0, 1)}
};

std::vector<std::pair<int8_t, int8_t>> int_shifts = {
  {-1, 0}, {0, -1}, {1, 0}, {0, 1}
};

struct Cell {
  int32_t rr;
  int32_t cc;
  char dir;
  char rot;
};


inline int32_t Code(int32_t rr, int32_t cc,
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
  } else if (dir_code == 1) {
    cell.dir = 'W';
  } else if (dir_code == 2) {
    cell.dir = 'S';
  } else if (dir_code == 3) {
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

std::vector<int32_t> GetNeighbours(int32_t code, int32_t rows, int32_t columns,
                                   const std::vector<std::vector<bool>>* strings, bool straight) {

  int32_t rot_code = code % 2;
  int32_t dir_code = (code / 2) % 4;
  int32_t cc_code = (code / 8) % columns;
  int32_t rr_code = (code / 8) / columns;
  // std::cout << "rr_code = " << rr_code << ", cc_code = " << cc_code << "\n";
  const std::vector<std::pair<int8_t, int8_t>>& possible_int_moves =
    int_movements[dir_code][rot_code];
  std::vector<int32_t> int_neighbours;
  for (const std::pair<int8_t, int8_t>& move : possible_int_moves) {
    int32_t n_int_dir = move.first;
    int32_t n_int_rot = move.second;
    // std::cout << "n_int_dir = " << n_int_dir << ", n_int_rot = " << n_int_rot << "\n";
    std::pair<int32_t, int32_t> int_shift;
    if (straight) {
      int_shift = int_shifts[n_int_dir];
    } else {
      int_shift = int_shifts[dir_code];
    }
    // std::cout << "int_shift = " << int_shift.first << ", " << int_shift.second << "\n";
    int32_t n_rr = rr_code;
    int32_t n_cc = cc_code;
    if (straight) {
      n_rr += int_shift.first;
      n_cc += int_shift.second;
    } else {
      n_rr -= int_shift.first;
      n_cc -= int_shift.second;
    }

    if (IsInside(n_rr, n_cc, rows, columns) && (*strings)[n_rr][n_cc]) {
      int_neighbours.push_back(n_int_rot + 2 * n_int_dir + 2 * 4 * n_cc + 2 * 4 * columns * n_rr);
    }
  }

/*
  Cell cell = Decoder(code, rows, columns);
  int32_t rr = cell.rr;
  int32_t cc = cell.cc;
  char dir = cell.dir;
  char rot = cell.rot;
  std::cout << "rr = " << rr << ", cc = " << cc << "\n";
  std::vector<int32_t> neighbours;
  std::string key = {dir, rot};
  std::vector<std::string> possible_moves = movements[key];
  for (std::string move : possible_moves) {
    char n_dir = move[0];
    char n_rot = move[1];
    std::cout << "n_dir = " << n_dir << ", n_rot = " << n_rot << "\n";
    std::pair<int32_t, int32_t> shift;
    if (straight) {
      shift = shifts[n_dir];
    } else {
      shift = shifts[dir];
    }
    int32_t n_rr = rr;
    int32_t n_cc = cc;
    if (straight) {
      n_rr += shift.first;
      n_cc += shift.second;
    } else {
      n_rr -= shift.first;
      n_cc -= shift.second;
    }
    std::cout << "shift = " << shift.first << ", " << shift.second << "\n";
    if (IsInside(n_rr, n_cc, rows, columns) && (*strings)[n_rr][n_cc]) {
      neighbours.push_back(Code(n_rr, n_cc,
                                n_dir, n_rot,
                                rows, columns));
    }
  }
  PrintVector(int_neighbours);
  PrintVector(neighbours);
  */
  return int_neighbours;
}


std::vector<int32_t> GetNeighbours(int rr, int cc,
                                   char dir, char rot,
                                   int32_t rows, int32_t columns,
                                   const std::vector<std::vector<bool>>& strings) {
  // return GetNeighbours(Code(rr, cc, dir, rot, rows, columns), rows, columns, &strings, true);
  std::vector<int32_t> neighbours;
  std::string key = {dir, rot};
  for (std::string move : movements[key]) {
    char n_dir = move[0];
    char n_rot = move[1];
    std::pair<int32_t, int32_t> shift = shifts[n_dir];
    int32_t n_rr = rr + shift.first;
    int32_t n_cc = cc + shift.second;
    if (IsInside(n_rr, n_cc, rows, columns) && strings[n_rr][n_cc]) {
      neighbours.push_back(Code(n_rr, n_cc,
                                n_dir, n_rot,
                                rows, columns));
    }
  }
  return neighbours;
}



class Path {
public:
  Path() {}

  Path(const std::set<int32_t> vertices,
       std::vector<int32_t>&& distances)
    : vertices_(vertices), distances_(distances) {}

  void AddStringsPtr(std::vector<std::vector<bool>>* strings_ptr) {
    strings_ptr_ = strings_ptr;
  }

  void SetRowsColumns(const int32_t rows, const int32_t columns) {
    rows_ = rows;
    columns_ = columns;
  }

  bool IsAvailable(int32_t finish_vertex) {
    return distances_[finish_vertex] < INF;
  }

  int32_t GetLength(int32_t finish_vertex) {
    return distances_[finish_vertex];
  }

  std::vector<int32_t> GetPath(int32_t finish_vertex) {
    std::vector<int32_t> path;
    path.push_back(finish_vertex);
    while (!vertices_.count(path.back())) {
      int32_t current_vertex = path.back();
      std::vector<int32_t> vertex_neighbours = GetNeighbours(current_vertex,
                                                             rows_, columns_,
                                                             strings_ptr_,
                                                             false);
      for (int32_t to : vertex_neighbours) {
        if (distances_[to] + 1 == distances_[current_vertex]) {
          path.push_back(to);
          break;
        }
      }
    }
    return path;
  }

private:
  std::set<int32_t> vertices_;
  std::vector<int32_t> distances_;

  std::vector<std::vector<bool>>* strings_ptr_;
  int32_t rows_;
  int32_t columns_;
};

class Graph {
public:
  explicit Graph (int32_t vertex_count) {
    vertex_count_ = vertex_count;
  }

  void AddStringsPtr(std::vector<std::vector<bool>>* strings_ptr) {
    strings_ptr_ = strings_ptr;
  }

  void SetRowsCols(const int32_t rows, const int32_t columns) {
    rows_ = rows;
    columns_ = columns;
  }

  Path GetBFSPaths(const std::set<int32_t>& vertices) {
    std::queue<int32_t> vertices_to_process;
    std::vector<int32_t> distances(vertex_count_, INF);
    // std::vector<bool> used(vertex_count_, false);
    for (const int32_t vertex : vertices) {
      vertices_to_process.push(vertex);
      distances[vertex] = 0;
      // used[vertex] = true;
    }
    // vertices_to_process.push(vertex);

    while (!vertices_to_process.empty()) {
      int32_t current_vertex = vertices_to_process.front();
      vertices_to_process.pop();
      std::vector<int32_t> vertex_neighbours =
        GetNeighbours(current_vertex,
                      rows_, columns_,
                      strings_ptr_, true);
      for (const int32_t to : vertex_neighbours) {
        if (distances[to] == INF) {
          // used[to] = true;
          distances[to] = distances[current_vertex] + 1;
          vertices_to_process.push(to);
        }
      }
    }
    Path paths = Path(vertices, std::move(distances));
    paths.AddStringsPtr(strings_ptr_);
    paths.SetRowsColumns(rows_, columns_);
    return paths;
  }

private:
  int32_t vertex_count_;
  std::vector<std::vector<bool>>* strings_ptr_;
  int32_t rows_;
  int32_t columns_;
};

Graph ConstructGraph(const int32_t rows,
                     const int32_t columns,
                     std::vector<std::vector<bool>>* strings_ptr) {
  Graph graph(rows * columns * 2 * 4);
  graph.SetRowsCols(rows, columns);
  graph.AddStringsPtr(strings_ptr);
  return graph;
}

Result Solve(const int32_t rows, const int32_t columns,
             std::vector<std::vector<bool>>* strings,
             const int32_t start_x, const int32_t start_y,
             const int32_t finish_x, const int32_t finish_y) {
  Graph graph = ConstructGraph(rows, columns, strings);
  int32_t minimal_path = 1000001;
  Path best_path;
  // int32_t best_start_code = Code(start_x, start_y, 'N', 'L', rows, columns);
  int32_t best_finish_code = Code(finish_x, finish_y, 'N', 'L', rows, columns);

  std::set<int32_t> vertices;

  for (char dir : {'N', 'W', 'S', 'E'}) {
    for (char rot : {'R', 'L'}) {
      int32_t current_start_code = Code(start_x, start_y, dir, rot, rows, columns);
      vertices.insert(current_start_code);
    }
  }

  Path paths = graph.GetBFSPaths(vertices);

  for (char f_dir : {'N', 'W', 'S', 'E'}) {
    for (char f_rot : {'R', 'L'}) {
      int32_t current_finish_code = Code(finish_x, finish_y, f_dir, f_rot, rows, columns);
      if (paths.GetLength(current_finish_code) < minimal_path) {
        minimal_path = paths.GetLength(current_finish_code);
        best_path = paths;
        best_finish_code = current_finish_code;
      }
    }
  }

  if (minimal_path < 1000001) {
    std::vector<std::pair<int32_t, int32_t>> cell_coords;
    std::vector<int32_t> best_codes = best_path.GetPath(best_finish_code);
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
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  int32_t rows = ReadInt32();
  int32_t columns = ReadInt32();
  std::vector<std::vector<bool>> strings = ReadStrings(rows, columns);
  int32_t start_x  = ReadInt32() - 1;
  int32_t start_y = ReadInt32() - 1;
  int32_t finish_x = ReadInt32() - 1;
  int32_t finish_y = ReadInt32() - 1;
  Result result = Solve(rows, columns,
                        &strings,
                        start_x, start_y,
                        finish_x, finish_y);
  PrintResult(result);
  return 0;
}
