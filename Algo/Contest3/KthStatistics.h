#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
#include <list>
#include <ctime>

template <typename T>
void printPointers(const std::vector<T*>& some_vector) {
    for (T* element : some_vector) {
        std::cerr << *element << "\n";
    }
}

template <typename T>
void printVector(const std::vector<T>& some_vector) {
    for (T element : some_vector) {
        std::cerr << element << "\n";
    }
    std::cerr << "\n";
}

template <typename T>
void printVector(const std::vector<std::vector<T>>& some_vector) {
    std::cerr << "vector vector:\n";
    for (auto sub_vector : some_vector) {
        std::cerr << ": ";
        for (T& element : sub_vector) {
            std::cerr << element << " ";
        }
        std::cerr << "\n";
    }
    std::cerr << "\n";
}


void readInput(int* len,
               int* query_count,
               int* kth,
               std::vector<int>* numbers,
               std::string* query,
               std::istream &in_stream = std::cin) {
    in_stream >> *len;
    in_stream >> *query_count;
    in_stream >> *kth;
    numbers->resize(*len);
    for (int i = 0; i < *len; ++i) {
        in_stream >> (*numbers)[i];
    }
    in_stream >> *query;
}


void writeOutput(const std::vector<int>& result,
                 std::ostream &out_stream = std::cout) {
    for (int x : result) {
        out_stream << x << "\n";
    }
}

class Fenwick {
private:
    std::vector<int> indicators_;
    int len_;

    int sum(int index) {
        int result = 0;
        for (; index >= 0; index = (index & (index + 1)) - 1)
            result += indicators_[index];
        return result;
    }

    void inc(int index, int delta) {
        for (; index < len_; index = (index | (index + 1)))
            indicators_[index] += delta;
    }

    int sum(int left, int right) {
        return sum(right) - sum(left - 1);
    }

public:
    explicit Fenwick(int _len) {
        len_ = _len;
        indicators_.assign(len_, 0);
    }

    void add(int element) {
        inc(element, 1);
    };

    void remove(int element) {
        inc(element, -1);
    };

    int getStatistics(int kth) {
        // std::cerr << "kth = " << kth << "\n";
        // std::cerr << sum(len_ - 1) << "\n";
        if (sum(len_ - 1) < kth) {
            return -2;
        }
        int lower_bound = -1;
        int upper_bound = len_ - 1;
        while (upper_bound - lower_bound > 1) {
            int mid_bound = (lower_bound + upper_bound) / 2;
            if (sum(mid_bound) < kth) {
                lower_bound = mid_bound;
            } else {
                upper_bound = mid_bound;
            }
        }

        return upper_bound;
    }
};

std::vector<int> solve(const int len,
                       const std::vector<int>& numbers,
                       const int kth,
                       const std::string& query) {
    std::vector<std::pair<int, int>> numbers_indexed;
    for (int i = 0; i < numbers.size(); ++i) {
        numbers_indexed.push_back(std::make_pair(numbers[i], i));
    }
    std::sort(numbers_indexed.begin(), numbers_indexed.end());

    std::vector<int> sorted_positions(len, 0);

    for (int i = 0; i < numbers_indexed.size(); ++i) {
        sorted_positions[numbers_indexed[i].second] = i;
    }

    Fenwick fenwick(len);
    // std::cerr << "add index " << sorted_positions[0] << "\n";
    fenwick.add(sorted_positions[0]);

    int left = 0;
    int right = 0;

    std::vector<int> result;

    for (char current_query : query) {
        if (current_query == 'L') {
            // std::cerr << "remove index " << sorted_positions[left] << "\n";
            fenwick.remove(sorted_positions[left]);
            ++left;
        } else {
            ++right;
            // std::cerr << "add index " << sorted_positions[right] << "\n";
            fenwick.add(sorted_positions[right]);
        }
        int statistics = fenwick.getStatistics(kth);
        // std::cerr << "statistics = " << statistics << "\n";
        if (statistics < 0) {
            result.push_back(-1);
        } else {
            int old_index = numbers_indexed[statistics].second;
            result.push_back(numbers[old_index]);
        }
    }

    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    int len;
    int query_count;
    int kth;
    std::vector<int> numbers;
    std::string query;
    readInput(&len, &query_count, &kth, &numbers, &query);
    std::vector<int> result = solve(len, numbers, kth, query);
    writeOutput(result);
}
