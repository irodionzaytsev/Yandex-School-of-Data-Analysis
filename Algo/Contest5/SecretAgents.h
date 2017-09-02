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

void readInput(int* agents_count,
               std::vector<std::pair<int64_t, int64_t>>* coordinates,
               std::istream &in_stream = std::cin) {
    in_stream >> *agents_count;
    for (int i = 0; i < *agents_count; ++i) {
        int64_t coord_x;
        int64_t coord_y;
        in_stream >> coord_x;
        in_stream >> coord_y;
        coordinates->push_back(std::make_pair(coord_x, coord_y));
    }
}

void writeOutput(long double answer,
                 std::ostream &out_stream = std::cout) {
    out_stream.precision(10);
    out_stream << std::fixed;
    out_stream << answer << "\n";
}

int64_t calcDist(std::pair<int64_t, int64_t> first_agent,
                 std::pair<int64_t, int64_t> second_agent) {
    return (first_agent.first - second_agent.first) * (first_agent.first - second_agent.first) +
           (first_agent.second - second_agent.second) * (first_agent.second - second_agent.second);
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

long double solve(const int agents_count,
                  const std::vector<std::pair<int64_t, int64_t>>& agents) {
    std::vector<std::pair<int64_t, std::pair<int, int>>> distant_edges;
    for (int first_agent = 0; first_agent < agents_count; ++first_agent) {
        for (int second_agent = first_agent + 1; second_agent < agents_count; ++second_agent) {
            distant_edges.push_back(
                    std::make_pair(
                            calcDist(agents[first_agent], agents[second_agent]),
                            std::make_pair(first_agent, second_agent)));
        }
    }
    int64_t maximal_dist = -1;
    std::sort(distant_edges.begin(), distant_edges.end());
    DSU dsu(agents_count);
    for (int i = 0; i < distant_edges.size(); ++i) {
        int first_set = dsu.find_set(distant_edges[i].second.first);
        int second_set = dsu.find_set(distant_edges[i].second.second);
        if (first_set != second_set) {
            maximal_dist = distant_edges[i].first;
        }
        dsu.union_sets(first_set, second_set);
    }
    return sqrt(maximal_dist);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    int agents_count;
    std::vector<std::pair<int64_t, int64_t>> coordinates;
    readInput(&agents_count, &coordinates);
    long double answer = solve(agents_count, coordinates);
    writeOutput(answer);
    return 0;
}
