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

void writeOutput(const std::vector<int>& prefix, std::ostream &out_stream = std::cout) {
  for (int p : prefix) {
    out_stream << p << " ";
  }
  out_stream << "\n";
}

std::vector<int> PFunction(const std::string& str) {
  std::vector<int> prefix(str.length(), 0);
  for (int i = 1; i < str.length(); ++i) {
    int curj = prefix[i - 1];
    while (curj > 0 && str[i] != str[curj]) {
      curj = prefix[curj - 1];
    }
    if (str[i] == str[curj]) {
      ++curj;
    }
    prefix[i] = curj;
  }
  return prefix;
}

int main() {

  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::string str = readInput();
  std::vector<int> prefix = PFunction(str);
  writeOutput(prefix);

  return 0;
}
