#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
const int SMALLCONST = -1000000001;

void readInput(int* n, int* m, std::vector<std::vector<int>>* grid,
               std::istream &in_stream = std::cin) {
    in_stream >> *n;
    in_stream >> *m;
    grid->assign(*n, std::vector<int> (*m, 0));
    for (int i = 0; i < *n; ++i) {
        for (int j = 0; j < *m; ++j) {
            in_stream >> (*grid)[i][j];
        }
    }
}

void writeOutput(const std::vector<int>& sequence, std::ostream &out_stream = std::cout) {
    for (size_t i = 0; i < sequence.size(); ++i) {
        out_stream << sequence[i] << " ";
    }
}

void printVector(const std::vector<int>& some_vector) {
    for (size_t i = 0; i < some_vector.size(); ++i) {
        std::cerr << some_vector[i] << " ";
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

template<typename T>
class Heap {

private:
    int size_;
    std::vector<T> elements_;

    bool heapify(int index) {
        if (2 * index + 1 >= size_) {
            return false;
        }
        if (2 * index + 2 >= size_) {
            if (elements_[index] > elements_[2 * index + 1]) {
                std::swap(elements_[index], elements_[2 * index + 1]);
                return true;
            }
            return false;
        }
        if (elements_[index] <= elements_[2 * index+1] && elements_[index] <= elements_[2 * index + 2]) {
            return false;
        }
        if (elements_[2 * index + 1] < elements_[2 * index + 2]) {
            std::swap(elements_[index], elements_[2 * index + 1]);
            return heapify(2 * index + 1);
        } else {
            std::swap(elements_[index], elements_[2 * index + 2]);
            return heapify(2 * index + 2);
        }
    }

public:
    Heap() {
        size_ = 0;
    }

    void add(T element) {
        ++size_;
        elements_.push_back(element);
        if (size_ == 1) {
            return;
        }
        bool flag = true;
        int current_index = (size_ - 2) / 2;
        while(flag) {
            flag = heapify(current_index);
            current_index = (current_index - 1) / 2;
        }
        //printVector(elements_);
    }

    T getMin() {
        //printVector(elements_);
        return elements_[0];
    }

    void pop() {
        std::swap(elements_[0], elements_[size_ - 1]);
        elements_.pop_back();
        --size_;
        heapify(0);
    }

    int getSize() {
        return size_;
    }

    bool isEmpty() {
        return elements_.empty();
    }

};

std::vector<int> solve(const int n, const int m, const std::vector<std::vector<int>>& grid) {
    Heap<std::pair<int, std::pair<int, int>>> heap;
    for (int i = 0; i < n; ++i) {
        heap.add(std::make_pair(grid[i][0], std::make_pair(i, 0)));
    }
    std::vector<int> sorted;
    while (!heap.isEmpty()) {
        auto minimalPair = heap.getMin();
        heap.pop();
        sorted.push_back(minimalPair.first);
        auto coordinates = minimalPair.second;
        int x = coordinates.first;
        int y = coordinates.second;
        if (coordinates.second + 1 < m) {
            heap.add(std::make_pair(grid[x][y + 1], std::make_pair(x, y + 1)));
        }
    }
    return sorted;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    int n, m;
    std::vector<std::vector<int>> grid;
    readInput(&n, &m, &grid);
    std::vector<int> sorted = solve(n, m, grid);
    writeOutput(sorted);
}
