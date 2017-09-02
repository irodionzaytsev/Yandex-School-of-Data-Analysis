
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