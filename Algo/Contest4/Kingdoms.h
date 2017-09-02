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


void readInput(int* city_count,
               std::vector<std::vector<int>>* first_graph,
               std::vector<std::vector<int>>* second_graph,
               std::istream &in_stream = std::cin) {
    in_stream >> *city_count;
    first_graph->resize(*city_count);
    second_graph->resize(*city_count);

    int point_a;
    int point_b;

    for (int i = 0; i + 1 < *city_count; ++i) {
        in_stream >> point_a;
        in_stream >> point_b;
        (*first_graph)[point_a - 1].push_back(point_b - 1);
        (*first_graph)[point_b - 1].push_back(point_a - 1);
    }

    for (int i = 0; i + 1 < *city_count; ++i) {
        in_stream >> point_a;
        in_stream >> point_b;
        (*second_graph)[point_a - 1].push_back(point_b - 1);
        (*second_graph)[point_b - 1].push_back(point_a - 1);
    }
}


void writeOutput(const int city_count,
                 const std::vector<int>& result,
                 std::ostream &out_stream = std::cout) {
    for (int i = 0; i < std::min((size_t)city_count, result.size()); ++i) {
        out_stream << result[i] + 1 << "\n";
    }
}

class VertexHash {
public:
    int64_t hash;
    int len;

    VertexHash() {
        len = 1;
        hash = 0;
    }

    VertexHash(int _len, int64_t _hash) :
        len(_len),
        hash(_hash) {};

    bool operator < (const VertexHash &other) const {
        return this->hash < other.hash;
    }

    friend std::ostream& operator<< (std::ostream& os, const VertexHash& obj) {
        os << obj.len << " -> " << obj.hash << "\n";
        return os;
    }
};

int64_t p_power(int64_t p, int degree, int64_t prime) {
    int64_t res = 1;
    while (degree) {
            if (degree & 1) {
                res *= p;
                res %= prime;
            }
            p = (p * p) % prime;
            degree >>= 1;
        }
    return res;
}


class Graph {

private:
    int root_;
    int graph_size_;
    std::vector<std::vector<int>> graph_;
    std::vector<int> sub_size_;
    std::vector<VertexHash> hashes;
    int64_t p = 2;
    // std::vector<int64_t> p_powers;
    int64_t prime = 1000000007;

    void dfs_init_(const int vertex,
                   const std::vector<std::vector<int>>& graph,
                   const int parent = -1) {

        for (int to : graph[vertex]) {
            if (to == parent) {
                continue;
            }
            graph_[vertex].push_back(to);
            dfs_init_(to, graph, vertex);
        }
    }

    void dfs_sub_size_(const int vertex, bool init = false) {
        if (init) {
            sub_size_.assign(graph_size_, 1);
        }
        for (int to : graph_[vertex]) {
            dfs_sub_size_(to);
            sub_size_[vertex] += sub_size_[to];
        }
    }

public:
    VertexHash concatenate(int vertex, std::vector<std::pair<VertexHash, int>> sub_hashes) {

        int64_t hash = graph_[vertex].size();

        std::sort(sub_hashes.begin(), sub_hashes.end());

        int len = 1;

        for (int i = 0; i < sub_hashes.size(); ++i) {
            VertexHash current_hash = sub_hashes[i].first;
            int current_index = sub_hashes[i].second;
            len += current_hash.len;
            hash = (hash + current_hash.hash * p_power(p, len - 1, prime)) % prime;
        }

        std::vector<int> indexes;

        for (int i = 0; i < sub_hashes.size(); ++i) {
            indexes.push_back(sub_hashes[i].second);
        }
        graph_[vertex] = indexes;

        return VertexHash(len, hash);
    }

    Graph(const std::vector<std::vector<int>>& graph, const int root = 0) {
        root_ = root;
        graph_size_ = graph.size();
        graph_.resize(graph_size_);
        dfs_init_(root, graph);
    }

    std::vector<std::vector<int>> reverse_graph(const std::vector<std::vector<int>>& graph) {
        std::vector<std::vector<int>> tmp_graph = graph;
        for (int i = 0; i < graph.size(); ++i) {
            for (int to : graph[i]) {
                tmp_graph[to].push_back(i);
            }
        }
        return tmp_graph;
    }

    Graph makeCentroidal() {
        dfs_sub_size_(root_, true);
        std::vector<int> centroids;
        for (int vertex = 0; vertex < graph_size_; ++vertex) {
            if (2 * sub_size_[vertex] < graph_size_) {
                continue;
            }

            bool little_children = true;
            for (int to : graph_[vertex]) {
                if (2 * sub_size_[to] > graph_size_) {
                    little_children = false;
                    break;
                }
            }
            if (little_children) {
                centroids.push_back(vertex);
            }
        }


        if (centroids.size() == 1) {
            return Graph(reverse_graph(graph_), centroids[0]);
        }

        int point_a = centroids[0];
        int point_b = centroids[1];
        graph_.push_back(std::vector<int>({}));
        graph_[graph_size_].push_back(point_a);
        graph_[graph_size_].push_back(point_b);

        for (std::vector<int>::iterator it = graph_[point_a].begin();
             it != graph_[point_a].end();
             ++it) {
            if (*it == point_b) {
                graph_[point_a].erase(it);
                break;
            }
        }
        for (std::vector<int>::iterator it = graph_[point_b].begin();
             it != graph_[point_b].end();
             ++it) {
            if (*it == point_a) {
                graph_[point_b].erase(it);
                break;
            }
        }

        return Graph(reverse_graph(graph_), graph_size_);
    }

    int getRoot() {
        return root_;
    }

    void list_vertice(std::vector<int>* correct_vertice_list, int vertex) {
        correct_vertice_list->push_back(vertex);
        for (int to : graph_[vertex]) {
            list_vertice(correct_vertice_list, to);
        }
    }

    void dfs_init_hashes(int vertex) {
        if (graph_[vertex].size() == 0) {
            return;
        }
        std::vector<std::pair<VertexHash, int>> sub_hashes;
        for (int to : graph_[vertex]) {
            dfs_init_hashes(to);
            sub_hashes.push_back(std::make_pair(hashes[to], to));
        }
        hashes[vertex] = concatenate(vertex, sub_hashes);
    }

    void init_hashes(int64_t _p, int64_t _prime) {
        p = _p;
        prime = _prime;
        // p_powers.push_back(int64_t(1));
        // for (int i = 0; i < 110000; ++i) {
        //     p_powers.push_back((p * p_powers.back()) % prime);
        // }
    }

    int64_t hashing() {
        init_hashes(1000003, 1000000009);
        hashes.resize(graph_size_);
        dfs_init_hashes(root_);

        return hashes[root_].hash;
    }
};


std::vector<int> solve(int city_count,
                       std::vector<std::vector<int>>* first_graph,
                       std::vector<std::vector<int>>* second_graph) {

    Graph first_centroidal = Graph(*first_graph).makeCentroidal();
    first_graph->clear();
    Graph second_centroidal = Graph(*second_graph).makeCentroidal();
    second_graph->clear();


    int64_t first_hash = first_centroidal.hashing();
    int64_t second_hash = second_centroidal.hashing();

    if (first_hash != second_hash) {
        return std::vector<int>({-2});
    }

    std::vector<int> first_graph_list;
    std::vector<int> second_graph_list;

    first_centroidal.list_vertice(&first_graph_list, first_centroidal.getRoot());
    second_centroidal.list_vertice(&second_graph_list, second_centroidal.getRoot());

    std::vector<int> result(first_graph_list.size());
    for (int i = 0; i < first_graph_list.size(); ++i) {
        result[first_graph_list[i]] = second_graph_list[i];
    }

    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    int city_count;
    std::vector<std::vector<int>> first_graph;
    std::vector<std::vector<int>> second_graph;
    readInput(&city_count, &first_graph, &second_graph);
    std::vector<int> result = solve(city_count, &first_graph, &second_graph);
    writeOutput(city_count, result);
}
