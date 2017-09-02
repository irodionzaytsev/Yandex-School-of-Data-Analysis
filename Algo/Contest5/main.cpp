#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
#include <list>
#include <ctime>
#include <sstream>

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

void readInput(int* vertex_count,
               std::vector<std::pair<int, std::pair<int, int>>>* coordinates,
               std::istream &in_stream = std::cin) {
    in_stream >> *vertex_count;
    int edges_count;
    in_stream >> edges_count;
    for (int i = 0; i < edges_count; ++i) {
        int coord_x;
        int coord_y;
        int weight;
        in_stream >> coord_x;
        in_stream >> coord_y;
        in_stream >> weight;
        coordinates->push_back(std::make_pair(weight, std::make_pair(coord_x - 1, coord_y - 1)));
    }
    std::sort(coordinates->begin(), coordinates->end());
}

void writeOutput(int weight,
                 std::ostream &out_stream = std::cout) {
    out_stream << weight << "\n";
}

class DSU {

public:
    explicit DSU(int count) {
        parent.resize(count);
        rank.resize(count);
        for (int i = 0; i < count; ++i) {
            make_set(i);
        }
    }

    void make_set (int vertex) {
        parent[vertex] = vertex;
        rank[vertex] = 0;
    }

    int find_set (int vertex) {
        if (vertex == parent[vertex])
            return vertex;
        return parent[vertex] = find_set (parent[vertex]);
    }

    void union_sets (int element_a, int element_b) {
        element_a = find_set (element_a);
        element_b = find_set (element_b);
        if (element_a != element_b) {
            if (rank[element_a] < rank[element_b])
                std::swap (element_a, element_b);
            parent[element_b] = element_a;
            if (rank[element_a] == rank[element_b])
                ++rank[element_a];
        }
    }

private:
    std::vector<int> parent;
    std::vector<int> rank;
};

int solve(const int vertex_count,
          const std::vector<std::pair<int, std::pair<int, int>>>& weighted_edges) {
    DSU dsu(vertex_count);
    int weight = -1;
    for (int i = 0; i < weighted_edges.size(); ++i) {
        int first_set = dsu.find_set(weighted_edges[i].second.first);
        int second_set = dsu.find_set(weighted_edges[i].second.second);
        if (first_set != second_set) {
            weight = weighted_edges[i].first;
        }
        dsu.union_sets(first_set, second_set);
    }
    return weight;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    int vertex_count;
    std::vector<std::pair<int, std::pair<int, int>>> weighted_edges;
    readInput(&vertex_count, &weighted_edges);
    int answer = solve(vertex_count, weighted_edges);
    writeOutput(answer);
    return 0;
}
