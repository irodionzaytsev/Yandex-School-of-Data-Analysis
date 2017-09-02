#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
#include <queue>

void readInput(int* children,
               int* kth,
               std::istream &in_stream = std::cin) {
    in_stream >> *children;
    in_stream >> *kth;
}

void writeOutput(const std::vector<int>& result,
                 std::ostream &out_stream = std::cout) {
    for (int element : result) {
        std::cout << element << " ";
    }
    std::cout << "\n";
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

    void increment(int index, int delta) {
        for (; index < len_; index = (index | (index + 1)))
            indicators_[index] += delta;
    }

public:
    explicit Fenwick(int _len) {
        len_ = _len;
        indicators_.assign(len_, 0);
    }

    void add(int element) {
        increment(element, 1);
    };

    void remove(int element) {
        increment(element, -1);
    };

    int getStatistics(int kth) {
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

std::vector<int> solve(int children, int kth) {
    Fenwick fenwick(children);
    for (int i = 0; i < children; ++i) {
        fenwick.add(i);
    }
    int index = 0;
    std::vector<int> result;
    for (int m = 0; m < children; ++m) {
        index = (index + kth - 1) % (children - m);
        // std::cerr << "index = " << index << "\n";
        int child = fenwick.getStatistics(index + 1);
        // std::cerr << "child = " << child << "\n";
        result.push_back(child + 1);
        fenwick.remove(child);
    }
    return result;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    int children;
    int kth;
    readInput(&children, &kth);
    std::vector<int> result = solve(children, kth);
    writeOutput(result);
}
