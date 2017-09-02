#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
const int SMALLCONST = -1000000001;

struct Cube {

    int first_;
    int second_;
    int third_;

    Cube(int first, int second, int third) :
            first_(first), second_(second), third_(third) {}

    bool operator < (const Cube& other) const {

        return (this->first_ < other.first_ &&
                this->second_ < other.second_ &&
                this->third_ < other.third_);
    }
};

void readInput(std::vector<Cube>* elements,
               std::istream &in_stream = std::cin) {
    int elements_count;
    in_stream >> elements_count;
    int first, second, third;
    for (int i = 0; i < elements_count; ++i) {
        in_stream >> first >> second >> third;
        (*elements).push_back(Cube(first, second, third));
    }
}

void writeOutput(int result, std::ostream &out_stream = std::cout) {
    out_stream << result;
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

int solve(const std::vector<Cube>& elements) {
    std::vector<int> dynamics(elements.size());
    for (int i = elements.size() - 1; i >= 0; --i) {
        dynamics[i] = 1;
        for (int j = elements.size() - 1; j > i; --j) {
            if (elements[i] < elements[j]) {
                dynamics[i] = std::max(dynamics[i], dynamics[j] + 1);
            }
        }
    }
    return *std::max_element(dynamics.begin(), dynamics.end());
}

int main() {
    std::vector<Cube> elements;
    readInput(&elements);
    int result = solve(elements);
    writeOutput(result);
}

