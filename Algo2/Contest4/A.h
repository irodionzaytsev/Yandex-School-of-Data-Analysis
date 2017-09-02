#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <map>
#include <set>

class Automaton {
public:
  explicit Automaton(const std::vector<std::vector<int>>& maper, int positions,
                     int alphabet, const std::vector<bool>& terminals)
    : maper_(maper), positions_(positions), alphabet_(alphabet), terminals_(terminals) {
    // current_position_ = 0
  }

  int GetMove(const int pos, const int letter) const {
    return maper_[pos][letter];
  }

  bool IsTerminal(const int pos) const {
    return terminals_[pos];
  }

  int GetSize() const {
    return positions_;
  }

  int AlphabetSize() const {
    return alphabet_;
  }

private:
  std::vector<std::vector<int>> maper_;
  int positions_;
  int alphabet_;
  // int current_position_;
  std::vector<bool> terminals_;
};

int ReadInt() {
  int number;
  std::cin >> number;
  return number;
}

int ReadChar() {
  char character;
  std::cin >> character;
  return character - 'a';
}

Automaton ReadAutomaton() {
  int size = ReadInt();
  int terminals_count = ReadInt();
  int letters_count = ReadInt();

  std::vector<bool> terminals(size, false);
  std::vector<std::vector<int>> maper(size, std::vector<int>(letters_count));

  for (int i = 0; i < terminals_count; ++i) {
    terminals[ReadInt()] = true;
  }

  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < letters_count; ++j) {
      int from = ReadInt();
      int with = ReadChar();
      int to = ReadInt();
      maper[from][with] = to;
    }
  }

  return Automaton(maper, size, letters_count, terminals);
}

bool BFS(Automaton& f_auto, Automaton& s_auto) {
  std::queue<std::pair<int, int>> positions;
  int alphabet_size = f_auto.AlphabetSize();
  // std::vector<bool> f_used(f_size, false);
  // std::vector<bool> s_used(s_size, false);
  std::set<std::pair<int, int>> used;

  positions.push(std::make_pair(0, 0));
  used.insert(std::make_pair(0, 0));

  while (!positions.empty()) {
    std::pair<int, int> current_positions = positions.front();
    positions.pop();
    int f_pos = current_positions.first;
    int s_pos = current_positions.second;
    // std::cout << "f, s " << f_pos << " " << s_pos << "\n";
    // f_used[f_pos] = true;
    // s_used[s_pos] = true;
    if (f_auto.IsTerminal(f_pos) != s_auto.IsTerminal(s_pos)) {
      return false;
    }

    for (int current_letter = 0; current_letter < alphabet_size; ++current_letter) {
      int next_f_pos = f_auto.GetMove(f_pos, current_letter);
      int next_s_pos = s_auto.GetMove(s_pos, current_letter);
      // if (!f_used[next_f_pos] || !s_used[next_s_pos]) {
      if (!used.count(std::make_pair(next_f_pos, next_s_pos))) {
        positions.push(std::make_pair(next_f_pos, next_s_pos));
        used.insert(std::make_pair(next_f_pos, next_s_pos));
      }
    }
  }
  return true;
}

void PrintResult(bool result) {
  if (!result) {
    std::cout << "NOT ";
  }
  std::cout << "EQUIVALENT\n";
}

int main() {

  for (int64_t p = 1; p < 200; ++p) {
    for (int64_t x = 1; x < 200; ++x) {
      if (p * p - p - 2 == 2 * x * x * x) {
        std::cout << p << " " << x << "\n";
      }
    }
  }


  Automaton f_auto = ReadAutomaton();
  Automaton s_auto = ReadAutomaton();

  bool result = BFS(f_auto, s_auto);

  PrintResult(result);

  return 0;
}
