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

class SuffixArray{
public:
  SuffixArray(std::string str, int alphabet_size) {
    str_ = str;
    alphabet_size_ = alphabet_size;
    zero_permutation.resize(str_.length());
    counter.assign(alphabet_size_, 0);
    zero_class.resize(str_.length());
    permutation.resize(str_.length());
    classe.resize(str_.length());
  }

  void ZeroPhase() {
    for (int i = 0; i < str_.size(); ++i) {
      ++counter[str_[i] - 'a'];
    }
    for (int i = 1; i < alphabet_size_; ++i) {
      counter[i] += counter[i - 1];
    }
    for (int i = 0; i < str_.size(); ++i) {
      // std::cout << str_[i] << " " << str_[i] - 'a' << " " << i << "\n";
      zero_permutation[--counter[str_[i] - 'a']] = i;
    }
    zero_class[zero_permutation[0]] = 0;
    classes = 1;
    for (int i = 1; i < str_.length(); ++i) {
      if (str_[zero_permutation[i]] != str_[zero_permutation[i - 1]]) {
        ++classes;
      }
        zero_class[zero_permutation[i]] = classes - 1;
    }
    /*
    std::cout << "\nPRINT\n";
    for (int i = 0; i < str_.length(); ++i) {
      std::cout << zero_permutation[i] << " ";
    }
    for (int i = 0; i < str_.length(); ++i) {
      std::cout << zero_class[i] << " ";
    }
    std::cout << "\n";
    */
  }

  void Phases() {
    for (int power = 0; (1 << power) < str_.length(); ++power) {
      for (int i = 0; i < str_.length(); ++i) {
        permutation[i] = zero_permutation[i] - (1 << power);
        if (permutation[i] < 0) {
          permutation[i] += static_cast<int>(str_.length());
        }
      }
      counter.assign(classes, 0);
      for (int i = 0; i < str_.length(); ++i) {
        ++counter[zero_class[permutation[i]]];
      }
      for (int i = 1; i < classes; ++i) {
        counter[i] += counter[i - 1];
      }
      for (int i = str_.length() - 1; i >= 0; --i) {
        zero_permutation[--counter[zero_class[permutation[i]]]] = permutation[i];
      }
      classe[zero_permutation[0]] = 0;
      classes = 1;
      for (int i = 1; i < str_.length(); ++i) {
        int midf = (zero_permutation[i] + (1 << power)) % str_.length();
        int mids = (zero_permutation[i - 1] + (1 << power)) % str_.length();
        if (zero_class[zero_permutation[i]] != zero_class[zero_permutation[i-1]] ||
          zero_class[midf] != zero_class[mids]) {
          ++classes;
        }
        classe[zero_permutation[i]] = classes - 1;
      }
      zero_class = classe;
      /*
      std::cout << "\nPRINT\n";
      for (int i = 0; i < str_.length(); ++i) {
        std::cout << permutation[i] << " ";
      }
      for (int i = 0; i < str_.length(); ++i) {
        std::cout << classe[i] << " ";
      }
      std::cout << "\n";
      */
    }
  }
  std::vector<int> GetPermutation() {
    std::vector<int> alt_permutation(permutation.size());
    std::vector<int> cunt(classe.size());
    // std::cout << "\nPRINT\n";
    for (int i = 0; i < classe.size(); ++i) {
      // std::cout << classe[i] << " ";
      ++cunt[classe[i]];
    }
    // std::cout << "\n";
    for (int i = 1; i < classe.size(); ++i) {
      cunt[i] += cunt[i - 1];
    }
    for (int i = 0; i < permutation.size(); ++i) {
      alt_permutation[--cunt[classe[i]]] = i;
    }
    /*
    for (int i = 0; i < permutation.size(); ++i) {
      std::cout << alt_permutation[i] << " ";
    }
    std::cout << "\n";
     */
    std::vector<int> inverse_permutation(permutation.size());
    for (int i = 0; i < permutation.size(); ++i) {
      inverse_permutation[alt_permutation[i]] = i;
    }
    /*
    for (int i = 0; i < permutation.size(); ++i) {
      std::cout << inverse_permutation[i] << " ";
    }
    std::cout << "\n";
     */
    // return permutation;
    return alt_permutation;
  }

private:
  std::vector<int> zero_permutation;
  std::vector<int> counter;
  std::vector<int> zero_class;
  std::vector<int> permutation;
  std::vector<int> classe;
  int classes;
  std::string str_;
  int alphabet_size_;
};

std::string ZIP(const std::string& str) {
  SuffixArray suffixArray(str, 26);
  // std::cerr << "ok1\n";
  suffixArray.ZeroPhase();
  // std::cerr << "ok2\n";
  suffixArray.Phases();
  // std::cerr << "ok3\n";
  // for (int len = 2; len <= 2 * str.length(); len *= 2) {
  //   suffixArray.Phase(len);
  // }
  std::vector<int> permutation = suffixArray.GetPermutation();
  std::string zipstr;
  // for (int index : permutation) {
  for (int i = 0; i < permutation.size(); ++i) {
    int index = permutation[i];
    // std::cout << "index = " << index << "\n";
    zipstr += str[(index + 2 * str.size() - 1) % str.size()];
  }
  // std::cerr << "ok4\n";
  // std::cerr << "zipstr:" << zipstr << "\n";
  return zipstr;
}

std::string GenStr(int len) {
  std::string str = "";
  for (int i = 0; i < len; ++i) {
    str += ('a' + rand() % 2);
  }
  return str;
}

/*
void Stress(int tests, int len) {
  for (int i = 0; i < tests; ++i) {
    std::cout << "test = " << i << "\n";
    std::string rand_string = GenStr(len);
    // td::cerr << "rand_string = " << rand_string << "\n";
    // std::cerr << ZIP(rand_string) << " heh\n";
    if (ZIP(rand_string) != Solve(rand_string)) {
      std::cerr << "wtf\n";
      std::cout << "ASSERT " << rand_string << "\n";
      std::cout << ZIP(rand_string) << "\n" << Solve(rand_string) << "\n";
      return;
    }
  }
}
*/

int main() {

  // clock_t start = clock();
  // Stress(10,  100000);
  // double duration = static_cast<double>(clock() - start) /  CLOCKS_PER_SEC;
  // std::cout << "duration = " << duration << "\n";

  // Stress(100, 100);

  std::string str = ReadString();
  std::string ans = ZIP(str);
  PrintString(ans);

  return 0;
}

