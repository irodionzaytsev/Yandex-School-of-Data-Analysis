#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <unordered_map>

std::string ReadString() {
  std::string str;
  std::cin >> str;
  return str;
}

void PrintString(const std::string& str, std::ostream &out_stream = std::cout) {
  out_stream << str << "\n";
}

using std::vector;

class SuffixArray2 {
public:
  explicit SuffixArray2(const std::string& string) {
    size_t size = string.size();
    sa.resize(size);
    vector<int> cls(size);
    vector<std::pair<int, int>> tmp(size);
    for (size_t i = 0; i < size; ++i) {
      tmp[i] = {string[i], i};
    }
    std::sort(tmp.begin(), tmp.end());
    for (size_t i = 0; i < size; ++i) {
      sa[i] = tmp[i].second;
      if (i > 0) {
        cls[sa[i]] = cls[sa[i - 1]] +
                     (tmp[i].first == tmp[i - 1].first ? 0 : 1);
      }
    }

    for (size_t len = 1; len < size; len *= 2) {
      size_t cl_cnt = cls[sa[size - 1]] + 1;
      if (cl_cnt == size) {
        break;
      }
      vector<int> new_sa(size);
      vector<int> cur(cl_cnt);
      for (size_t i = 0; i < size; ++i) {
        if (i == 0 || cls[sa[i]] != cls[sa[i - 1]]) {
          cur[cls[sa[i]]] = i;
        }
      }

      for (size_t i = 0; i < size; ++i) {
        int index = sa[i] - len;
        if (index < 0) {
          index += size;
        }
        int cl = cls[index];
        new_sa[cur[cl]++] = index;
      }

      vector<int> new_cls(size);
      sa.swap(new_sa);
      for (size_t index = 1; index < size; ++index) {
        size_t first_i = sa[index] + len;
        size_t second_i = sa[index - 1] + len;
        if (first_i >= size) {
          first_i -= size;
        }
        if (second_i >= size) {
          second_i -= size;
        }
        if (cls[sa[index]] == cls[sa[index - 1]] && cls[first_i] == cls[second_i]) {
          new_cls[sa[index]] = new_cls[sa[index - 1]];
        } else {
          new_cls[sa[index]] = new_cls[sa[index - 1]] + 1;
        }
      }
      cls.swap(new_cls);
    }

    pos.resize(size);
    for (size_t i = 0; i < size; ++i) {
      pos[sa[i]] = i;
    }
    for (size_t i = 1; i < size - 1; ++i) {
      size_t index = pos[i] + 1;
      if (index == size) {
        continue;
      } else {
        index = sa[index];
      }
    }
  }

  vector<int> getPositions() const {
    return sa;
  }

private:
  vector<int> sa;
  vector<int> pos;
};

std::string Solve(const std::string& string) {
  SuffixArray2 array(string);
  auto positions = array.getPositions();
  std::string result = "";
  for (size_t pos : positions) {
    result += string[(pos + string.length() - 1) % string.length()];
  }
  return result;
}

class SuffixArray{
public:
  explicit SuffixArray(const std::string& str) :
    str_(str) {
    permutation_.assign(str.length(), 0);
    classes_.assign(str.length(), 0);
  }

  void ZeroPhase() {
    std::map<char, int> char_counter;
    for (char character : str_) {
      ++char_counter[character];
    }
    std::map<char, int> map_index;
    std::map<char, int> leclass;
    int sum = 0;
    int current_class = 0;
    for (auto it : char_counter) {
      map_index[it.first] = sum;
      leclass[it.first] = current_class;
      sum += it.second;
      ++current_class;
    }
    for (int index = 0; index < str_.length(); ++index) {
      char character = str_[index];
      permutation_[index] = map_index[character]++;
      // ++map_index[character];
      classes_[index] = leclass[character];
    }
    // Print();
  };

  struct Hash {
    std::size_t operator()(const std::pair<int, int> &obj) const {
      return obj.first ^ obj.second;
    }
  };

  void Phase(int len) {
    std::map<std::pair<int, int>, int> pair_counter;
    for (int i = 0; i < str_.length(); ++i) {
      ++pair_counter[std::make_pair(classes_[i], classes_[(i + len / 2) % str_.length()])];
    }
    std::unordered_map<std::pair<int, int>, int, Hash> map_pair_index;
    map_pair_index.reserve(str_.length());
    std::unordered_map<std::pair<int, int>, int, Hash> leclass;
    leclass.reserve(str_.length());
    int sum = 0;
    int current_class = 0;
    for (auto it : pair_counter) {
      map_pair_index[it.first] = sum;
      leclass[it.first] = current_class;
      sum += it.second;
      ++current_class;
    }

    std::vector<int> classes_cpy(classes_.size());
    for (int i = 0; i < str_.length(); ++i) {
      std::pair<int, int> current_pair = std::make_pair(classes_[i],
                                                        classes_[(i + len / 2) % str_.length()]);
      permutation_[i] = map_pair_index[current_pair]++;
      // ++map_pair_index[current_pair];
      classes_cpy[i] = leclass[current_pair];
      // classes_[i] = leclass[current_pair];
    }
    classes_ = classes_cpy;
    // Print();
  }

  std::vector<int> GetPermutation() {
    std::vector<int> inverse_permutation(permutation_.size());
    for (int i = 0; i < permutation_.size(); ++i) {
      inverse_permutation[permutation_[i]] = i;
    }
    return inverse_permutation;
  }

  void Print() {
    std::cout << "\nPRINT\n";
    for (int i = 0; i < permutation_.size(); ++i) {
      std::cout << permutation_[i] << " ";
    }
    std::cout << "\n";
    for (int i = 0; i < classes_.size(); ++i) {
      std::cout << classes_[i] << " ";
    }
    std::cout << "\n";
  }

private:
  std::string str_;
  std::vector<int> permutation_;
  std::vector<int> classes_;
};

std::string ZIP(const std::string& str) {
  SuffixArray suffixArray(str);
  suffixArray.ZeroPhase();
  for (int len = 2; len <= 2 * str.length(); len *= 2) {
    suffixArray.Phase(len);
  }
  std::vector<int> permutation = suffixArray.GetPermutation();
  std::string zipstr;
  for (int index : permutation) {
    // std::cout << "index = " << index << "\n";
    zipstr += str[(index + str.size() - 1) % str.size()];
  }
  return zipstr;
}


std::string GenStr(int len) {
  std::string str = "";
  for (int i = 0; i < len; ++i) {
    str += ('a' + rand() % 2);
  }
  return str;
}

void Stress(int tests, int len) {
  for (int i = 0; i < tests; ++i) {
    // std::cout << "test = " << i << "\n";
    std::string rand_string = GenStr(len);
    if (ZIP(rand_string) != ZIP(rand_string)) {
      std::cout << "ASSERT " << rand_string << "\n";
      std::cout << ZIP(rand_string) << "\n" << Solve(rand_string) << "\n";
      return;
    }
  }
}


int main() {

  clock_t start = clock();
  Stress(1, 100000);
  double duration = static_cast<double>(clock() - start) /  CLOCKS_PER_SEC;



  std::cout << "duration = " << duration << "\n";


  std::string str = ReadString();
  std::string ans = ZIP(str);
  PrintString(ans);

  return 0;
}
