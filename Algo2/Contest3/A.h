#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>

std::string ReadString() {
  std::string str;
  std::cin >> str;
  return str;
}

void PrintAnswer(const std::vector<int>& zf) {
  for (int i = 0; i < zf.size(); ++i) {
    std::cout << zf[i] << "\n";
  }
}

struct state {
  int len;
  int link;
  std::map<char, int> next;
};

class Kurwa{
public:
  Kurwa() {
    size = 1;
    last = 0;
    states.push_back({0, -1});
  }

  template <class Callback>
  void Extend (char c, Callback on_match) {
    // std::cout << "c = " << c << "\n";
    int old_max_len = 0;
    if (states[last].link > -1) {
      old_max_len = states[states[last].link].len;
    }
    int cur = size;
    ++size;
    // std::cout << "size = " << size << "\n";
    // std::cout << "cur = " << cur << "\n";
    states.push_back({states[last].len + 1, 0});
    // states[cur].len = states[last].len + 1;
    int p_link;
    for (p_link = last;
         p_link != -1 && !states[p_link].next.count(c);
         p_link = states[p_link].link) {
      states[p_link].next[c] = cur;
    }
    if (p_link == -1) {
      states[cur].link = 0;
    } else {
      int q_link = states[p_link].next[c];
      if (states[p_link].len + 1 == states[q_link].len) {
        states[cur].link = q_link;
      } else {
        int clone = size++;

        // states[clone].len = states[p_link].len + 1;
        // states[clone].next = states[q_link].next;
        // states[clone].link = states[q_link].link;
        states.push_back({states[p_link].len + 1, states[q_link].link, states[q_link].next});
        for (; p_link != -1 && states[p_link].next[c] == q_link; p_link = states[p_link].link) {
          states[p_link].next[c] = clone;
        }
        states[q_link].link = states[cur].link = clone;
      }
    }
    last = cur;
    int new_max_len = states[states[last].link].len;
    // std::cerr << new_max_len << " " << old_max_len << "\n";
    if (new_max_len <= old_max_len) {
      on_match(old_max_len, new_max_len);
    }
  }

private:
  std::vector<state> states;
  size_t size;
  int last;
};

std::vector<int32_t> LZArray(const std::string& str) {
  Kurwa kurwa = Kurwa();
  std::vector<int> answers(str.length(), 0);
  for (int index = 0; index < str.length(); ++index) {
    char character = str[index];
    kurwa.Extend(character,
                 [&answers, &index](int old_len, int new_len) {
                   int pos_f = index - old_len;
                   int pos_s = index - new_len + 1;
                   int cur = old_len;
                   while (pos_f < pos_s) {
                     answers[pos_f++] = cur--;
                   }
                 });
  }
  // PrintAnswer(answers);
  std::vector<int> reduced(answers.begin(), --answers.end());
  return reduced;
}


std::string RandString(int len) {
  std::string rand_string;
  for (int i = 0; i < len; ++i) {
    rand_string += ('a' + rand() % 2);
  }
  return rand_string;
}

int Check(const std::string& str, int index_i, int index_j) {
  int ans = 0;
  for (int len = 0; index_i + len < str.length(); ++len) {
    if (str[index_i + len] == str[index_j + len]) {
      ++ans;
    } else {
      break;
    }
  }
  return ans;
}

std::vector<int> EasySolve(std::string str) {
  std::vector<int> answers(str.length(), 0);
  for (int i = 0; i < str.length(); ++i) {
    for (int j = 0; j < i; ++j) {
      answers[i] = std::max(answers[i], Check(str, i, j));
    }
  }
  return std::vector<int>(answers.begin(), --answers.end());
}

void Stress(int tests, int len) {
  for (int i = 0; i < tests; ++i) {
    std::cout << "test = " << i << "\n";
    std::string rand_string = RandString(len) + "#";
    if (EasySolve(rand_string) != LZArray(rand_string)) {
      std::cout << "WRONG TEST: " << rand_string << "\n";
      PrintAnswer(EasySolve(rand_string));
      std::cout << "VS\n";
      PrintAnswer(LZArray(rand_string));
      return;
    }
  }
}

int main() {

  // Stress(400, 10);

  const std::string str = ReadString() + "#";
  const std::vector<int32_t> maxes = LZArray(str);
  PrintAnswer(maxes);

  return 0;
}
