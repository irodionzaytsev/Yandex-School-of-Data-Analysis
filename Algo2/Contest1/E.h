#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
#include <iomanip>

void printVector(const std::vector<int>& some_vector) {
  for (size_t i = 0; i < some_vector.size(); ++i) {
    std::cerr << some_vector[i] << " ";
  }
  std::cerr << std::endl;
}

template<typename T>
void printVectorVector(const std::vector<std::vector<T>>& some_vector) {
  std::cerr << "\n";
  for (auto& sub_vector : some_vector) {
    for (auto &element : sub_vector) {
      element.print();
    }
    std::cerr << std::endl;
  }
  std::cerr << std::endl;
}

template<typename T>
void printVector(const std::vector<T>& some_vector) {
  std::cerr << "\n";
  for (auto &element : some_vector) {
    element.print();
  }
  std::cerr << std::endl;
}

void printBoolVector (const std::vector<bool>& some_vector) {
  std::cerr << "true indexes are:\n";
  for (int i = 0; i < some_vector.size(); ++i) {
    if (some_vector[i]) {
      std::cerr << i << " ";
    }
  }
  std::cerr << std::endl;
}

void printPairVector(const std::vector<std::pair<int, int>>& some_vector) {
  for (size_t i = 0; i < some_vector.size(); ++i) {
    std::cerr << "{ " << some_vector[i].first << ", " << some_vector[i].second << "} ";
  }
  std::cerr << std::endl;
}

std::string readInput(std::istream &in_stream = std::cin) {
  std::string str;
  in_stream >> str;
  return str;
}

void writeOutput(const std::vector<int>& answer, std::ostream &out_stream = std::cout) {
  std::cout << answer.size() << "\n";
  for (int x : answer) {
    std::cout << x + 1 << " ";
  }
}

std::vector<int> ZFunction(const std::string& str) {
  std::vector<int> zed(str.length(), 0);
  int left = 0;
  int right = 0;
  for (int i = 1; i < str.length(); ++i) {
    if (i <= right) {
      zed[i] = std::min(right - i + 1, zed[i - left]);
    }
    while (i + zed[i] < str.length() && str[zed[i]] == str[i + zed[i]]) {
      ++zed[i];
    }
    if (i + zed[i] - 1 >= right) {
      left = i;
      right = i + zed[i] - 1;
    }
  }
  zed[0] = str.length();
  return zed;
}

std::vector<int> Solve(const std::string& f_str, const std::string& s_str) {
  std::string rf_str = f_str;
  std::string rs_str = s_str;
  std::reverse(rf_str.begin(), rf_str.end());
  std::reverse(rs_str.begin(), rs_str.end());
  std::string f_templ = f_str + "#" + s_str;
  std::string r_templ = rf_str + "#" + rs_str;
  std::vector<int> f_zed = ZFunction(f_templ);
  std::vector<int> r_zed = ZFunction(r_templ);
  f_zed = std::vector<int>(f_zed.begin() + f_str.length() + 1, f_zed.end());
  r_zed = std::vector<int>(r_zed.begin() + rf_str.length() + 1, r_zed.end());
  std::reverse(r_zed.begin(), r_zed.end());
  std::vector<int> answers;
  for (int i = 0; i + f_str.length() - 1 < s_str.length(); ++i) {
    if (f_zed[i] + r_zed[i + f_str.length() - 1] >= f_str.length() - 1) {
      answers.push_back(i);
    }
  }
  return answers;
}

int CountErrors(const std::string& f_str, const std::string& s_str) {
  int errs = 0;
  for (int i = 0; i < std::max(f_str.length(), s_str.length()); ++i) {
    if (f_str[i] != s_str[i]) {
      ++errs;
    }
  }
  return errs;
}

std::vector<int> EasySolve(const std::string& f_str, const std::string& s_str) {

  std::vector<int> answers;

  for (int i = 0; i + f_str.length() - 1 < s_str.length(); ++i) {
    if (CountErrors(f_str, s_str.substr(i, f_str.length())) <= 1) {
      answers.push_back(i);
    }
  }
  return answers;
}

std::string GenString(int len) {
  std::string str;
  for (int i = 0; i < len; ++i) {
    int ran = rand() % 3;
    if (ran % 3 == 0) {
      str += 'a';
    } else if (ran % 3 == 1) {
      str += 'b';
    } else {
      str += 'c';
    }
  }
  return str;
}

void UnitTest(int f_len, int s_len, int tests) {
  for (int i = 0; i < tests; ++i) {
    std::string f_str = GenString(f_len);
    std::string s_str = GenString(s_len);
    if (Solve(f_str, s_str) != EasySolve(f_str, s_str)) {
      std::cout << f_str << " " << s_str << "\n";
      printVector(Solve(f_str, s_str));
      printVector(EasySolve(f_str, s_str));
    }
  }
}

int main() {
  // UnitTest(6, 1000, 1000);
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::string strp = readInput();
  std::string strt = readInput();
  std::vector<int> answer = Solve(strp, strt);
  writeOutput(answer);

  return 0;
}
