
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <set>

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


std::vector<int32_t> LZArray(const std::string& str) {
  std::set<char> letters;
  std::set<std::string> bigrams;
  std::vector<int> partial_answers(str.length(), -1);

  /*
  partial_answers[0] = 0;
  letters.insert(str[0]);
  if (str.length() > 1) {
    bigrams.insert("" + str[0] + str[1]);
  }
*/
  for (int i = 0; i < str.length(); ++i) {
    if (!letters.count(str[i])) {
      partial_answers[i] = 0;
      letters.insert(str[i]);
      if (i + 1 < str.length()) {
        bigrams.insert(str.substr(i, 2));
      }
      continue;
    }

    std::cout << "does it? " << str.substr(i, 2) << ", " << bigrams.count(str.substr(i, 2)) << "\n";

    if ((i + 1 < str.length() && !bigrams.count(str.substr(i, 2))) || (i + 1 == str.length())) {
      partial_answers[i] = 1;
    }
    if (i + 1 < str.length()) {
      letters.insert(str[i]);
      bigrams.insert(str.substr(i, 2));
    }
  }
  PrintAnswer(partial_answers);

  for (int i = static_cast<int>(partial_answers.size()) - 1; i >= 0; --i) {
    if (partial_answers[i] == -1) {
      partial_answers[i] = partial_answers[i + 1] + 1;
    }
  }

  return partial_answers;
}

std::string RandString(int len) {
  std::string rand_string;
  for (int i = 0; i < len; ++i) {
    rand_string += ('a' + rand() % 2);
  }
  return rand_string;
}

int Check(const std::string& str, int i, int j) {
  int ans = 0;
  for (int len = 0; i + len < str.length(); ++len) {
    if (str[i + len] == str[j + len]) {
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
  return answers;
}

void Stress(int tests, int len) {
  for (int i = 0; i < tests; ++i) {
    std::cout << "test = " << i << "\n";
    std::string rand_string = RandString(len);
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

  Stress(40, 6);

  const std::string str = ReadString();
  const std::vector<int32_t> maxes = LZArray(str);
  PrintAnswer(maxes);

  return 0;
}
