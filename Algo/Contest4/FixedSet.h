#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
#include <list>
#include <ctime>
#include <set>

const int BITS = 32;
const int MAX = 1000000000;

void readInput(std::vector<int>* set_numbers,
               std::vector<int>* queries,
               std::istream &in_stream = std::cin) {
    int set_numbers_size;
    in_stream >> set_numbers_size;
    set_numbers->reserve(set_numbers_size);
    for (int i = 0;i < set_numbers_size; ++i) {
        int element;
        in_stream >> element;
        set_numbers->push_back(element);
    }
    int queries_size;
    in_stream >> queries_size;
    for (int i = 0; i < queries_size; ++i) {
        int element;
        in_stream >> element;
        queries->push_back(element);
    }
}

void writeOutput(const std::vector<std::string>& result,
                 std::ostream &out_stream = std::cout) {
    for (auto& answer : result) {
        out_stream << answer << "\n";
    }
}


class Vertex {
private:
    int next_vertice_index[2];
    bool is_leaf;

public:
    Vertex();

    bool isLeaf();

    void setLeaf(bool leaf);

    int getNext(int letter);

    void setNext(int letter, int index);
};


class FixedSet {
private:
    std::vector<Vertex> vertice;
    void addNumber(int element);

public:
    FixedSet();

    void initialize(const std::vector<int>& set_numbers);

    bool contains(int element);
};


std::vector<std::string> solve(const std::vector<int>& set_numbers,
                               const std::vector<int>& queries) {
    FixedSet fixedSet;
    fixedSet.initialize(set_numbers);
    std::vector<std::string> result;
    for (int element : queries) {
        if (fixedSet.contains(element)) {
            result.push_back("Yes");
        } else {
            result.push_back("No");
        }
    }
    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::vector<int> set_numbers;
    std::vector<int> queries;
    readInput(&set_numbers, &queries);
    std::vector<std::string> result = solve(set_numbers, queries);
    writeOutput(result);
}

Vertex::Vertex() {
    for (int i = 0; i < 2; ++i) {
        next_vertice_index[i] = -1;
    }
    is_leaf = false;
}

bool Vertex::isLeaf() {
    return is_leaf;
}

void Vertex::setLeaf(bool leaf) {
    is_leaf = leaf;
}

int Vertex::getNext(int letter) {
    return next_vertice_index[letter];
}

void Vertex::setNext(int letter, int index) {
    next_vertice_index[letter] = index;
}

FixedSet::FixedSet() {
    vertice.push_back(Vertex());
}

void FixedSet::addNumber(int element) {
    int current_vertex_index = 0;
    for (int i = 0; i < BITS; ++i) {
        int direction = element % 2;
        element /= 2;
        int next_vertex_index = vertice[current_vertex_index].getNext(direction);
        if (next_vertex_index == -1) {
            vertice[current_vertex_index].setNext(direction, vertice.size());
            next_vertex_index = vertice.size();
            vertice.push_back(Vertex());
        }
        current_vertex_index = next_vertex_index;
    }
    vertice[current_vertex_index].setLeaf(true);
}

void FixedSet::initialize(const std::vector<int> &set_numbers) {
    vertice.reserve(set_numbers.size());
    for (int element : set_numbers) {
        addNumber(element + MAX);
    }
}

bool FixedSet::contains(int element) {
    element += MAX;
    int current_vertex_index = 0;
    for (int i = 0; i < BITS; ++i) {
        int direction = element % 2;
        element /= 2;
        int next_vertex_index = vertice[current_vertex_index].getNext(direction);
        if (next_vertex_index == -1) {
            return false;
        }
        current_vertex_index = next_vertex_index;
    }
    return vertice[current_vertex_index].isLeaf();
}
