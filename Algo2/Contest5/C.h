#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <queue>
#include <unordered_set>
#include <string>
#include <unordered_map>

void PrintResult(const int32_t result) {
  std::cout << result << "\n";
}

int32_t ReadInt32() {
  int32_t num;
  std::cin >> num;
  return num;
}

int32_t StrToGrid(const std::string& str) {
  int32_t answer = 0;
  int32_t curdeg = 1;
  for (char character : str) {
    answer += (character - '0') * curdeg;
    curdeg *= 2;
  }
  return answer;
}

std::string ArrToStr(const std::vector<std::string>& strs) {
  std::string grid;
  for (const std::string& str : strs) {
    for (char character : str) {
      grid += character;
    }
  }
  return grid;
}

std::string ArrToStr(const std::vector<std::vector<char>>& strs) {
  std::string grid;
  for (auto& str : strs) {
    for (const char& character : str) {
      grid += character;
    }
  }
  return grid;
}

int32_t ReadGrid(const int& width, const int& height) {
  std::string grid;
  for (int i = 0; i < width; ++i) {
    std::string str;
    std::cin >> str;
    grid += str;
  }
  // std::cout << "grid str = " << grid << "\n";
  return StrToGrid(grid);
}

char OppChar(char character) {
  if (character == '0') {
    return '1';
  } else {
    return '0';
  }
}

std::pair<std::string, std::string> GetFinishes(const int& width, const int& height) {
  std::vector<std::string> finishes;
  for (char start : {'0', '1'}) {
    std::vector<std::vector<char>> grid(width, std::vector<char> (height));
    for (int i = 0; i < width; ++i) {
      for (int j = 0; j < height; ++j) {
        // std::cout << "i, j = " << i << " " << j << "\n";
        if ((i + j) % 2 == 0) {
          grid[i][j] = start;
        } else {
          grid[i][j] = OppChar(start);
        }
      }
    }
    finishes.push_back(ArrToStr(grid));
  }

  return std::make_pair(finishes[0], finishes[1]);
};

std::pair<int32_t, int32_t> GetIntFinishes(const int& width, const int& height) {
  auto strFinishes = GetFinishes(width, height);
  // std::cout << "strs = " << strFinishes.first << " " << strFinishes.second << "\n";
  return std::make_pair(StrToGrid(strFinishes.first), StrToGrid(strFinishes.second));
};

int32_t code(const int height, const int o_x, const int o_y) {
  return o_x * height + o_y;
}

std::vector<int32_t> Edges(const int width, const int height) {

  std::vector<int32_t> edges;

  for (int i = 0; i + 1 < width; ++i) {
    for (int j = 0; j < height; ++j) {
      edges.push_back((1 << code(height, i, j)) + (1 << code(height, i + 1, j)));
    }
  }

  for (int i = 0; i < width; ++i) {
    for (int j = 0; j + 1 < height; ++j) {
      edges.push_back((1 << code(height, i, j)) + (1 << code(height, i, j + 1)));
    }
  }

  return edges;
}

int32_t BFS(const int width,
            const int height,
            const int grid,
            const std::pair<int32_t, int32_t> finishes) {
  int32_t f_finish = finishes.first;
  int32_t s_finish = finishes.second;
  std::vector<int32_t> edges = Edges(width, height);
  // std::queue<std::pair<int32_t, int32_t>> positions;
  std::queue<int32_t> positions;
  std::unordered_map<int32_t, int8_t> used;
  used.reserve(1 << 16);
  // std::set<int32_t> used;
  positions.push(grid);
  used[grid] = 1;
  while (!positions.empty()) {
    auto mask = positions.front();
    int32_t dist = used[mask];
    positions.pop();
    // int32_t mask = position.first;
    // int32_t dist = position.second;
    if (mask == f_finish || mask == s_finish) {
      return used[mask] - 1;
    }
    for (int32_t edge : edges) {
      int32_t next = mask ^ edge;
      // auto unit = used.insert(std::make_pair(next, 0));
      // if (used.find(next) == used.end()) {
      if (used[next] == 0) {
        // used[next] = 1;
        used[next] = dist + 1;
        positions.push(next);
      }
    }
  }
  return -1;
}

int main() {
  int width = ReadInt32();
  int height = ReadInt32();
  int32_t grid = ReadGrid(width, height);
  // std::cout << "grid = " << grid << "\n";
  std::pair<int32_t, int32_t> finishes = GetIntFinishes(width, height);
  // std::cout << "finishes = " << finishes.first << " " << finishes.second << "\n";
  clock_t start = clock();
  int32_t result = BFS(width, height, grid, finishes);
  std::cout << (clock() - start) / 1.0 / CLOCKS_PER_SEC << "\n";
  PrintResult(result);

  return 0;
}
