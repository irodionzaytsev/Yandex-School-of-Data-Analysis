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

void readInput(int* count,
               std::vector<std::pair<char, int>>* requests,
               std::istream &in_stream = std::cin) {
    in_stream >> *count;
    int request_count;
    in_stream >> request_count;
    for (int i = 0; i < request_count; ++i) {
        char operation;
        int number;
        in_stream >> operation >> number;
        requests->push_back(std::make_pair(operation, number));
    }
}

void writeOutput(const std::vector<int> answer,
                 std::ostream &out_stream = std::cout) {
    for (int i = 0; i < answer.size(); ++i) {
        out_stream << answer[i] << "\n";
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

    int get(int element) {
        return sum(element, element);
    }

    void remove(int element) {
        inc(element, -1);
    };

    int firstZeroIndex(int first, int last) {
        if (sum(first, last - 1) == last - first) {
            return -1;
        }
        if (sum(first, first) == 0) {
            return first;
        }
        int lower_bound = first;
        int upper_bound = last - 1;

        while(upper_bound - lower_bound > 1) {
            int mid_bound = (lower_bound + upper_bound) / 2;
            if (sum(first, mid_bound) == mid_bound - first + 1) {
                lower_bound = mid_bound;
            } else {
                upper_bound = mid_bound;
            }
        }
        return upper_bound;
    }
};

std::vector<int> solve(const int count, const std::vector<std::pair<char, int>>& requests) {

    Fenwick fenwick(count);
    std::vector<int> result;

    for (const auto& request : requests) {
        char operation = request.first;
        int number = request.second - 1;
        // std::cerr << operation  << " " << number << "\n";
        if (operation == '+') {
            int index = fenwick.firstZeroIndex(number, count);
            if (index < 0) {
                index = fenwick.firstZeroIndex(0, number);
            }
            if (index < 0) {
                // std::cerr << "kurwa\n";
                result.push_back(-1);
            } else {
                fenwick.add(index);
                result.push_back(index + 1);
            }
        }
        if (operation == '-') {
            if (fenwick.get(number) == 0) {
                result.push_back(-2);
            } else {
                fenwick.remove(number);
                result.push_back(0);
            }
        }
    }
    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    int count;
    std::vector<std::pair<char, int>> requests;
    readInput(&count, &requests);
    std::vector<int> answer = solve(count, requests);
    writeOutput(answer);
    return 0;
}
