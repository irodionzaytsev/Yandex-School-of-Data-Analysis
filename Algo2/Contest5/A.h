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

const int MAX_RULES_COUNT = 150;
const int LETTERS_COUNT = 26;

void PrintResult(const std::string& str) {
  std::cout << str << "\n";
}

int32_t ReadInt32() {
  int32_t num;
  std::cin >> num;
  return num;
}

std::map<char, std::vector<std::string>> ReadRules() {
  std::map<char, std::vector<std::string>> rules;
  int rules_count = ReadInt32();
  for (int i = 0; i < rules_count; ++i) {
    std::string str;
    std::cin >> str;
    rules[str[0]].push_back(str.substr(3, static_cast<int>(str.length()) - 3));
  }
  return rules;
};

bool IsLower(char character) {
  return 'a' <= character && character <= 'z';
}

bool IsUpper(char character) {
  return 'A' <= character && character <= 'Z';
}

bool IsEps(char character) {
  return character == '$';
}

bool IsFinalRule(const std::string& rule, const std::set<char>& adequates) {
  for (char character : rule) {
    if (IsUpper(character) && !adequates.count(character)) {
      return false;
    }
  }
  return true;
}

std::set<char> Adequates(const std::map<char, std::vector<std::string>>& rules) {
  std::set<char> adequates;
  for (int i = 0; i < MAX_RULES_COUNT; ++i) {
    for (auto mapit : rules) {
      char symbol = mapit.first;
      for (auto rule : mapit.second) {
        if (IsFinalRule(rule, adequates)) {
          adequates.insert(symbol);
          continue;
        }
      }
    }
  }
  return adequates;
};

std::map<char, std::vector<std::string>> Filtre(
  const std::map<char, std::vector<std::string>>& rules, const std::set<char>& adequates) {
  std::map<char, std::vector<std::string>> filtered_rules;
  for (const auto& mapit : rules) {
    for (const auto& rule : mapit.second) {
      if (IsFinalRule(rule, adequates)) {
        filtered_rules[mapit.first].push_back(rule);
      }
    }
  }
  return filtered_rules;
};

std::string Possibles(const std::map<char, std::vector<std::string>>& rules) {
  auto adequate_sybmols = Adequates(rules);
  auto filtered_rules = Filtre(rules, adequate_sybmols);

  /*
  for (const auto& mapit : filtered_rules) {
    for (const auto& rule : mapit.second) {
      std::cout << mapit.first << "->" << rule << "\n";
    }
  }
   */

  std::map<char, std::set<char>> possible_begins;

  for (int it = 0; it <= 3 * LETTERS_COUNT * LETTERS_COUNT; ++it) {
    for (const auto &mapit : filtered_rules) {
      char symbol = mapit.first;
      for (const auto &rule : mapit.second) {
        // std::cout << "symbol = " << symbol << "\n";
        // std::cout << "rule = " << rule << "\n";
        bool eps_flag = true;
        int index = 0;
        for (; index < rule.length() && IsUpper(rule[index]) && eps_flag; ++index) {
          char current_symbol = rule[index];
          // std::cout << "cur = " << current_symbol << "\n";
          for (char possible_begin : possible_begins[current_symbol]) {
            possible_begins[symbol].insert(possible_begin);
            // std::cout << "so that " << possible_begin << "\n";
          }
          eps_flag = possible_begins[current_symbol].count('$');
          // std::cout << "eps_flag = " << eps_flag << "\n";
        }
        if (index < rule.length() && !IsUpper(rule[index]) && eps_flag) {
          possible_begins[symbol].insert(rule[index]);
        }
      }
    }
  }

  std::string answer = "";

  for (char character : possible_begins['S']) {
    if (IsLower(character)) {
      answer += character;
    }
  }
  std::sort(answer.begin(), answer.end());

  return answer;
}

int main() {

  std::map<char, std::vector<std::string>> rules = ReadRules();
  std::string posses = Possibles(rules);
  PrintResult(posses);

  return 0;
}



