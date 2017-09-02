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


std::pair<std::vector<std::pair<int, int>>,
        std::vector<std::pair<int, int>>> makeDynamicPair(const std::vector<int>& elements) {
    std::vector<std::pair<int, int>> increase_dynamics;
    std::vector<std::pair<int, int>> decrease_dynamics;
    increase_dynamics.push_back(std::make_pair(1, 0));
    decrease_dynamics.push_back(std::make_pair(1, 0));
    for (size_t current_element = 1; current_element < elements.size(); ++current_element) {
        std::pair<int, int> best_increase_pair = std::make_pair(0, -current_element);
        std::pair<int, int> best_decrease_pair = std::make_pair(0, -current_element);
        for (size_t j = 0; j < current_element; ++j) {
            if (elements[j] > elements[current_element]) {
                std::pair<int, int> new_j_pair = increase_dynamics[j];
                new_j_pair.second = -j;
                best_decrease_pair = std::max(best_decrease_pair, new_j_pair);
            }
            if (elements[j] < elements[current_element]) {
                std::pair<int, int> new_j_pair = decrease_dynamics[j];
                new_j_pair.second = -j;
                best_increase_pair = std::max(best_increase_pair, new_j_pair);
            }
        }
        increase_dynamics.push_back(std::make_pair(best_increase_pair.first + 1,
                                                   best_increase_pair.second));
        decrease_dynamics.push_back(std::make_pair(best_decrease_pair.first + 1,
                                                   best_decrease_pair.second));
    }
    return std::make_pair(increase_dynamics, decrease_dynamics);
};

std::vector<bool> getCriticalIndexes(int ans,
                                     const std::vector<int> elements,
                                     const std::vector<std::pair<int, int>>& increase_dynamics,
                                     const std::vector<std::pair<int, int>>& decrease_dynamics) {
    bool is_first = true;
    std::vector<bool> criticals(increase_dynamics.size(), false);
    for (int i = increase_dynamics.size() - 1; i >= 0; --i) {
        if (increase_dynamics[i].first == ans || decrease_dynamics[i].second == ans) {
            criticals[i] = true;
        }
        if (!criticals[i]) {
            continue;
        }
        for (int j = i - 1; j >= 0; --j) {
            if (elements[j] > elements[i] && decrease_dynamics[i].first == increase_dynamics[j].first + 1) {
                criticals[j] = true;
            }
            if (elements[j] < elements[i] && increase_dynamics[i].first == decrease_dynamics[j].first + 1) {
                criticals[j] = true;
            }
        }
    }
    return criticals;
}

/*
int getLastIndex(int ans, const std::vector<std::pair<int, int>>& dynamics) {
    for (int i = 0; i < dynamics.size(); ++i) {
        if (dynamics[i].first == ans) {
            return i;
        }
    }
    return -1;
}

std::vector<int> getJumps(  const std::vector<int>& elements,
                            const std::vector<std::pair<int, int>>& first_dynamics,
                            const std::vector<std::pair<int, int>>& second_dynamics) {
    std::vector<bool> critical_points = getCriticalIndexes(ans, elements, first_dynamics, second_dynamics);
}
*/
std::vector<int> bestIndexes(int ans,
                             const std::vector<int>& elements,
                             const std::vector<std::pair<int, int>>& increase_dynamics,
                             const std::vector<std::pair<int, int>>& decrease_dynamics) {
    std::vector<bool> critical_points = getCriticalIndexes(ans, elements, increase_dynamics, decrease_dynamics);
    printBoolVector(critical_points);
    int current_index = 0;
    int current_length = 1;
    while(!critical_points[current_index]) {
        ++current_index;
    }
    std::cerr << "current_index=" << current_index << std::endl;
    std::vector<int> indexes;
    indexes.push_back(current_index);
    for(int next_index = indexes.back() + 1; next_index < elements.size(); ++next_index) {
        std::cerr << "next_index=" << next_index << std::endl;
        if (!critical_points[next_index]) {
            continue;
        }
        if (elements[next_index] > elements[current_index]) {
            if (increase_dynamics[next_index].first == decrease_dynamics[current_index].first + 1) {
                bool flag = false;
                if (indexes.size() < 2) {
                    flag = true;
                } else {
                    int prev_prev = elements[indexes[indexes.size() - 2]];
                    if (is_convex(prev_prev, elements[current_index], elements[next_index])) {
                        flag = true;
                    }
                }
                if (flag) {
                    indexes.push_back(next_index);
                    current_index = next_index;
                }
            }
        } else if (elements[next_index] < elements[current_index]) {
            if (decrease_dynamics[next_index].first == increase_dynamics[current_index].first + 1) {
                bool flag = false;
                if (indexes.size() < 2) {
                    flag = true;
                } else {
                    int prev_prev = elements[indexes[indexes.size() - 2]];
                    if (is_convex(prev_prev, elements[current_index], elements[next_index])) {
                        flag = true;
                    }
                }
                if (flag) {
                    indexes.push_back(next_index);
                    current_index = next_index;
                }
            }
        }
    }
    return indexes;
}
/*
std::vector<int> chooseBestIndexes(const std::vector<int>& best_increase_indexes,
                                   const std::vector<int>& best_decrease_indexes) {
    if (best_decrease_indexes.size() < best_increase_indexes.size()) {
        return best_increase_indexes;
    }
    if (best_increase_indexes.size() < best_decrease_indexes.size()) {
        return best_decrease_indexes;
    }
    for (int i = 0; i < best_increase_indexes.size(); ++i) {
        if (best_increase_indexes[i] < best_decrease_indexes[i]) {
            return best_increase_indexes;
        }
        if (best_increase_indexes[i] > best_decrease_indexes[i]) {
            return best_decrease_indexes;
        }
    }
    return best_increase_indexes;
}
*/
std::vector<int> solve(const std::vector<int>& elements) {
    std::pair<std::vector<std::pair<int, int>>,
            std::vector<std::pair<int, int>>> dynamic_pair =
            makeDynamicPair(elements);
    std::vector<std::pair<int, int>> increase_dynamics = dynamic_pair.first;
    std::vector<std::pair<int, int>> decrease_dynamics = dynamic_pair.second;
    printPairVector(increase_dynamics);
    printPairVector(decrease_dynamics);
    int answer = std::max(increase_dynamics.back().first, decrease_dynamics.back().first);
    std::vector<int> best_indexes = bestIndexes(answer,
                                                elements,
                                                increase_dynamics,
                                                decrease_dynamics);
    return makeSequence(best_indexes, elements);
}

int main() {
    std::vector<int> elements;
    readInput(&elements);
    std::vector<int> sequence = solve(elements);
    writeOutput(sequence);
}
