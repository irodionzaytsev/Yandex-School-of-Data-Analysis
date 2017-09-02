#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
const int SMALLCONST = -1000000001;

void readInput(std::vector<int>* elements,
               std::istream &in_stream = std::cin) {
    int elements_count;
    in_stream >> elements_count;
    int element;
    for (int i = 0; i < elements_count; ++i) {
        in_stream >> element;
        (*elements).push_back(element);
    }
}

void writeOutput(const std::vector<int>& sequence, std::ostream &out_stream = std::cout) {
    for (size_t i = 0; i < sequence.size(); ++i) {
        out_stream << sequence[i] << " ";
    }
}

bool is_convex(int first, int second, int third) {
    return (
            (second > first && second > third) ||
            (second < first && second < third)
    );
}

std::vector<int> makeSequence(const std::vector<int>& sequence_indexes,
                              const std::vector<int>& elements) {
    std::vector<int> sequence;
    for (int i = 0; i < sequence_indexes.size(); ++i) {
        sequence.push_back(elements[sequence_indexes[i]]);
    }
    return sequence;
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

std::vector<int> getSequence(const std::vector<int> first_links,
                             const std::vector<int> second_links) {
    bool turn = true;
    std::vector<int> sequence;
    int current_index = 0;
    sequence.push_back(current_index);
    int next_index = first_links[current_index];
    while (next_index != current_index) {
        turn = !turn;
        sequence.push_back(next_index);
        current_index = next_index;
        if (turn) {
            next_index = first_links[current_index];
        } else {
            next_index = second_links[current_index];
        }
    }
    return sequence;
}

std::vector<int> chooseBestSequence(std::vector<int> first_sequence,
                                    std::vector<int> second_sequence) {
    if (first_sequence.size() > second_sequence.size()) {
        return first_sequence;
    }
    if (first_sequence.size() < second_sequence.size()) {
        return second_sequence;
    }
    for (int i = 0; i < first_sequence.size(); ++i) {
        if (first_sequence[i] < second_sequence[i]) {
            return first_sequence;
        }
        if (first_sequence[i] > second_sequence[i]) {
            return second_sequence;
        }
    }
    return first_sequence;
}

std::vector<int> solve(const std::vector<int>& elements) {
    std::vector<int> increase_dynamics(elements.size());
    std::vector<int> decrease_dynamics(elements.size());
    std::vector<int> increase_links(elements.size());
    std::vector<int> decrease_links(elements.size());
    for (int index = elements.size() - 1; index >= 0; --index) {
        increase_dynamics[index] = 1;
        decrease_dynamics[index] = 1;
        increase_links[index] = index;
        decrease_links[index] = index;
        for (int current_index = elements.size() - 1; current_index > index; --current_index) {
            if (elements[index] > elements[current_index]) {
                if (decrease_dynamics[index] <= increase_dynamics[current_index] + 1) {
                    decrease_dynamics[index] = increase_dynamics[current_index] + 1;
                    decrease_links[index] = current_index;
                }
            }
            if (elements[index] < elements[current_index]) {
                if (increase_dynamics[index] <= decrease_dynamics[current_index] + 1) {
                    increase_dynamics[index] = decrease_dynamics[current_index] + 1;
                    increase_links[index] = current_index;
                }
            }
        }
    }
    std::vector<int> increase_sequence = getSequence(increase_links, decrease_links);
    std::vector<int> decrease_sequence = getSequence(decrease_links, increase_links);
    std::vector<int> best_indexes = chooseBestSequence(increase_sequence, decrease_sequence);
    return makeSequence(best_indexes, elements);
}

int main() {
    std::vector<int> elements;
    readInput(&elements);
    std::vector<int> sequence = solve(elements);
    writeOutput(sequence);
}
