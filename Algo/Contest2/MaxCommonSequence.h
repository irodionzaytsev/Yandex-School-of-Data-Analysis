#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
const int SMALLCONST = -1000000001;

void readInput(size_t* first_count,
               size_t* second_count,
               std::vector<int>* first_sequence,
               std::vector<int>* second_sequence,
               std::istream &in_stream = std::cin) {
    in_stream >> *first_count;
    (*first_sequence).resize(*first_count);
    for (int i = 0; i < *first_count; ++i) {
        in_stream >> (*first_sequence)[i];
    }

    in_stream >> *second_count;
    (*second_sequence).resize(*second_count);
    for (int i = 0; i < *second_count; ++i) {
        in_stream >> (*second_sequence)[i];
    }
}

void writeOutput(int result, std::ostream &out_stream = std::cout) {
    out_stream << result << std::endl;
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

int getIndex(const std::vector<std::vector<int>>& dynamics, int index_i, int index_j) {
    if (index_i < 0 || index_j < 0) {
        return 0;
    }
    return dynamics[index_i][index_j];
}

int solve(const int first_count,
          const int second_count,
          const std::vector<int>& first_sequence,
          const std::vector<int>& second_sequence) {
    std::vector<std::vector<int>> dynamics(first_count, std::vector<int> (second_count, 0));
    dynamics[0][0] = 0;
    for (int i = 0; i < first_count; ++i) {
        for (int j = 0; j < second_count; ++j) {
            if (first_sequence[i] == second_sequence[j]) {
                dynamics[i][j] = getIndex(dynamics, i - 1, j - 1) + 1;
            } else {
                dynamics[i][j] = std::max(getIndex(dynamics, i - 1, j),
                                          getIndex(dynamics, i, j - 1));
            }
        }
    }
    return dynamics[first_count - 1][second_count - 1];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    size_t first_count, second_count;
    std::vector<int> first_sequence;
    std::vector<int> second_sequence;
    readInput(&first_count, &second_count, &first_sequence, &second_sequence);
    int result = solve(first_count, second_count, first_sequence, second_sequence);
    writeOutput(result);
}
