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

template <typename T>
void printPointers(const std::vector<T*>& some_vector) {
    for (T* element : some_vector) {
        std::cerr << *element << "\n";
    }
}

void printBoolVector(const std::vector<bool>& some_vector) {
    std::cerr << "print bool vector\n";
    for (int i = 0; i < some_vector.size(); ++i) {
        if (some_vector[i]) {
            std::cerr << i << " ";
        }
    }
    std::cerr << "\n";
}

template <typename T>
void printVector(const std::vector<T>& some_vector) {
    for (T element : some_vector) {
        std::cerr << element << " ";
    }
    std::cerr << "\n";
}

void printBoolVector(const std::vector<std::vector<bool>>& some_vector) {
    std::cerr << "bool vector vector:\n";
    for (int i = 0; i < some_vector.size(); ++i) {
        std::cerr <<  i << " : ";
        for (int j = 0; j < some_vector[i].size(); ++j) {
            if (some_vector[i][j]) {
                std::cerr << j << " ";
            }
        }
        std::cerr << "\n";
    }
}


template <typename T>
void printVector(const std::vector<std::vector<T>>& some_vector) {
    std::cerr << "vector vector:\n";
    std::cerr << "vector.size() : " << some_vector.size() << "\n";
    int index = 0;
    for (auto sub_vector : some_vector) {
        std::cerr << index << ": ";
        ++index;
        for (T element : sub_vector) {
            std::cerr << element << " ";
        }
        std::cerr << "\n";
    }
    std::cerr << "\n";
}

void readInput(int* city_count,
               std::vector<std::vector<int>>* graph,
               std::istream &in_stream = std::cin) {
    in_stream >> *city_count;
    graph->resize(*city_count);

    int point_a;
    int point_b;

    for (int i = 0; i + 1 < *city_count; ++i) {
        in_stream >> point_a;
        in_stream >> point_b;
        (*graph)[point_a - 1].push_back(point_b - 1);
        (*graph)[point_b - 1].push_back(point_a - 1);
    }
}

void writeOutput(const std::vector<int>& result,
                 std::ostream &out_stream = std::cout) {
    out_stream << result.size() << "\n";
    for (int dist : result) {
        out_stream << dist << "\n";
    }
}

std::vector<long long> makeUnion(const std::vector<std::vector<bool>>& sets) {
    size_t max_size = 0;
    for (int i = 0; i < sets.size(); ++i) {
        max_size = std::max(max_size, sets[i].size());
    }
    std::vector<long long> sets_union(max_size, 0);
    for (int i = 0; i < sets.size(); ++i) {
        for (int j = 0; j < sets[i].size(); ++j) {
            if (sets[i][j]) {
                ++sets_union[j];
            }
        }
    }
    return sets_union;
}

std::vector<long long> sum(const std::vector<long long>& set_a, const std::vector<long long>& set_b) {
    std::vector<long long> set_c (set_a.size() + set_b.size(), 0);
    for (int i = 0; i < set_a.size(); ++i) {
        for (int j = 0; j < set_b.size(); ++j) {
            set_c[i + j] += set_a[i] * set_b[j];
        }
    }
    return set_c;
}

void decrement(std::vector<long long>* set_a, std::vector<long long> set_b) {
    for (int i = 0; i < set_b.size(); ++i) {
        (*set_a)[i] -= set_b[i];
    }
}

std::vector<int> setToInt(const std::vector<bool>& set) {
    std::vector<int> set_int(set.size(), 0);
    for (int i = 0; i < set.size(); ++i) {
        if (set[i]) {
            ++set_int[i];
        }
    }
    return set_int;
}

std::vector<bool> setToBool(const std::vector<long long>& set) {
    std::vector<bool> set_bool(set.size(), 0);
    for (int i = 0; i < set.size(); ++i) {
        if (set[i] > 0) {
            set_bool[i] = true;
        }
    }
    return set_bool;
}

std::vector<bool> fastSum(const std::vector<std::vector<bool>>& sets) {
    // std::cerr << "fastSum!\n";
    // std::cerr << "sets\n";
    // printBoolVector(sets);
    std::vector<long long> sets_union = makeUnion(sets);
    // std::cerr << "sets_union\n";
    // printVector(sets_union);
    // std::vector<long long> union_sum = sum(sets_union, sets_union);
    //std::cerr << "union_sum\n";
    //printVector(union_sum);
    for (int i = 0; i < sets.size(); ++i) {
        decrement(&union_sum, sum(setToInt(sets[i]), setToInt(sets[i])));
    }
    return setToBool(union_sum);
}

std::vector<bool> concatenateBoolSets(const std::vector<std::vector<bool>>& sets) {
    size_t max_size = 0;
    for (auto& set : sets) {
        max_size = std::max(max_size, set.size());
    }
    std::vector<bool> concat(max_size, false);
    for (auto& set : sets) {
        for (int element = 0; element < set.size(); ++element) {
            if (set[element]) {
                concat[element] = true;
            }
        }
    }
    return concat;
}

std::vector<bool> translate(const std::vector<bool>& paths, const int translation) {
    std::vector<bool> translated(paths.size() + translation, false);
    for(int i = 0; i < paths.size(); ++i) {
        translated[i + translation] = paths[i];
    }
    return translated;
}

class Graph {

private:
    int root_;
    int graph_size_;
    Graph* graph_ptr_;
    std::vector<std::vector<int>> graph_;
    std::vector<int> sub_size_;

    void dfs_init_(const int vertex, const std::vector<std::vector<int>>& graph, const int parent = -1) {

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

    void dfs_subtree_(const int vertex, int* vertex_alias, std::vector<std::vector<int>>* subtree) const{
        // std::cerr << "subtree_ " << vertex << " " << *vertex_alias << "\n";
        // printVector(*subtree);
        // std::cerr << "hm heh\n";
        int current_alias = *vertex_alias;
        for (int to : graph_[vertex]) {
            ++(*vertex_alias);
            (*subtree)[current_alias].push_back(*vertex_alias);
            subtree->push_back(std::vector<int>({}));
            // subtree->back().push_back(current_alias);
            dfs_subtree_(to, vertex_alias, subtree);
        }
    }

    void dfs_path_lens_(const int vertex, std::vector<bool>* bool_set_path_lens, int current_len) const {
        if (graph_[vertex].size() == 0) {
            while (bool_set_path_lens->size() <= current_len) {
                bool_set_path_lens->push_back(false);
            }
            (*bool_set_path_lens)[current_len] = true;
        } else {
            for (int to : graph_[vertex]) {
                dfs_path_lens_(to, bool_set_path_lens, current_len + 1);
            }
        }
    }

    Graph getSubtree_(int vertex) const {
        std::vector<std::vector<int>> subtree(1);
        int vertex_alias = 0;
        dfs_subtree_(vertex, &vertex_alias, &subtree);
        // std::cerr << "reversing\n";
        // printVector(subtree);
        // printVector(reverse_graph_(subtree));

        return Graph(reverse_graph_(subtree), graph_ptr_);
    }

    std::vector<std::vector<int>> reverse_graph_(const std::vector<std::vector<int>>& graph) const {
        std::vector<std::vector<int>> tmp_graph = graph;
        for (int i = 0; i < graph.size(); ++i) {
            for (int to : graph[i]) {
                tmp_graph[to].push_back(i);
            }
        }
        return tmp_graph;
    }

public:

    Graph(const std::vector<std::vector<int>>& graph, Graph* graph_ptr, const int root = 0) {
        if (graph_ptr == nullptr) {
            graph_ptr_ = this;
        } else {
            graph_ptr_ = graph_ptr;
        }
        root_ = root;
        graph_size_ = graph.size();
        graph_.resize(graph_size_);
        dfs_init_(root, graph);
        dfs_sub_size_(root_, true);
    }

    Graph getCentroidal() {
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

        return Graph(reverse_graph_(graph_), graph_ptr_, centroids[0]);
    }

    bool isLeaf(int vertex) {
        if (vertex == root_) {
            return (graph_[vertex].size() < 2);
        } else {
            return (graph_[vertex].size() == 0);
        }
    }

    std::vector<bool> getPathLens() const {
        std::vector<bool> bool_set_path_lens;
        dfs_path_lens_(root_, &bool_set_path_lens, 0);
        return bool_set_path_lens;
    }

    std::vector<bool> getLens() const {
        if (graph_size_ == 1) {
            if (graph)
            return std::vector<bool>(1, true);
        }
        std::vector<std::vector<bool>> set_lens;
        std::vector<std::vector<bool>> set_paths;
        for (int to : graph_[root_]) {
            Graph sub_graph = getSubtree_(to).getCentroidal();
            // sub_graph.print(30);
            set_lens.push_back(sub_graph.getLens());
            set_paths.push_back(translate(sub_graph.getPathLens(), 1));
        }

        std::cerr << "graph size is " << graph_size_ << "\n";

        std::cerr << "set_paths\n";
        printBoolVector(set_paths);

        std::vector<bool> sum_sub_path_lens = fastSum(set_paths);
        std::cerr << "sum_sub_path_lens\n";
        printBoolVector(sum_sub_path_lens);
        std::cerr << "printd\n";
        set_lens.push_back(sum_sub_path_lens);
        std::cerr << "set_lens\n";
        printBoolVector(set_lens);

        std::vector<bool> lens = concatenateBoolSets(set_lens);
        std::cerr << "lens\n";
        printBoolVector(lens);
        return lens;
    }

    void print(const int translation, std::ostream &os = std::cerr) {
        std::string translation_s = "";
        for (int i = 0; i < translation; ++i) {
            translation_s.push_back(' ');
        }
        os << translation_s << "the graph describing:\n";
        os << translation_s << " root : " << root_ << "\n";
        os << translation_s << " edges:\n";
        for (int vertex = 0; vertex < graph_size_; ++vertex) {
            os << translation_s << "vertex : " << vertex << "\n";
            for (int to : graph_[vertex]) {
                os << translation_s << " -- " << to << "\n";
            }
        }
        os << translation_s << "sub sizes:\n";
        for (int i = 0; i < sub_size_.size(); ++i) {
            os << translation_s << "vertex : " <<i << " -> " << sub_size_[i] << "\n";
        }
        os << translation_s << "path lens:\n";
        std::vector<bool> pathLens = getPathLens();
        for (int i = 0; i < pathLens.size(); ++i) {
            os << translation_s;
            if (pathLens[i]) {
                os << i << " ";
            }
        }
        os << translation_s << "\n";
        os << translation_s << "leafs distances:\n";
        std::vector<bool> dists = getLens();
        // os << translation_s << "dists.size() = " << dists.size() << "\n";
        os << translation_s;
        for (int i = 0; i < dists.size(); ++i) {

            if (dists[i]) {
                os << i << " ";
            }
        }
        os << translation_s << "\n";

    }

    friend std::ostream& operator<< (std::ostream& os, const Graph& graph) {
        os << "the graph describing:\n";
        os << " root : " << graph.root_ << "\n";
        os << " edges:\n";
        for (int vertex = 0; vertex < graph.graph_size_; ++vertex) {
            os << "vertex : " << vertex << "\n";
            for (int to : graph.graph_[vertex]) {
                os << " -- " << to << "\n";
            }
        }
        os << "sub sizes:\n";
        for (int i = 0; i < graph.sub_size_.size(); ++i) {
            os << "vertex : " <<i << " -> " << graph.sub_size_[i] << "\n";
        }
        os << "path lens:\n";
        std::vector<bool> pathLens = graph.getPathLens();
        for (int i = 0; i < pathLens.size(); ++i) {
            if (pathLens[i]) {
                os << i << " ";
            }
        }
        os << "\n";
        os << "leafs distances:\n";
        std::vector<bool> dists = graph.getLens();
        for (int i = 0; i < dists.size(); ++i) {
            if (dists[i]) {
                os << i << " ";
            }
        }
        return os;
    }
};


std::vector<int> solve(int city_count,
                       std::vector<std::vector<int>> edges) {
    Graph graph = Graph(edges, nullptr).getCentroidal();
    //std::cerr  << graph << "\n";
    graph.print(10);
    // return std::vector<int>();
    std::vector<bool> lens = graph.getLens();
    lens[0] = false;
    std::vector<int> result;
    for (int len = 0; len < lens.size(); ++len) {
        if (lens[len]) {
            result.push_back(len);
        }
    }
    return result;
}

std::vector<int>generatePrufer(int graph_size) {
    std::vector<int> code;
    for (int i = 0; i + 2 < graph_size; ++i) {
        code.push_back(rand() % graph_size);
    }
    return code;
}

std::vector<std::vector<int>> pruferDecode (const std::vector<int> & prufer_code) {
    int graph_size = prufer_code.size() + 2;
    std::vector<int> degree (graph_size, 1);
    for (int i=0; i<graph_size - 2; ++i)
        ++degree[prufer_code[i]];

    std::set<int> leaves;
    for (int i=0; i<graph_size; ++i)
        if (degree[i] == 1)
            leaves.insert (i);

    std::vector<std::pair<int,int>> result;
    for (int i=0; i<graph_size - 2; ++i) {
        int leaf = *leaves.begin();
        leaves.erase (leaves.begin());

        int vertex = prufer_code[i];
        result.push_back (std::make_pair (leaf, vertex));
        if (--degree[vertex] == 1)
            leaves.insert (vertex);
    }
    result.push_back(std::make_pair (*leaves.begin(), *--leaves.end()));
    std::vector<std::vector<int>> graph (graph_size);
    for (auto edge : result) {
        graph[edge.first].push_back(edge.second);
        graph[edge.second].push_back(edge.first);
    }
    return graph;
}

std::vector<int> easy_solve(int city_count, const std::vector<std::vector<int>>& graph) {
    std::vector<std::vector<int>> matrix(city_count, std::vector<int>(city_count, city_count + 2));
    for (int i = 0; i < graph.size(); ++i) {
        for (int to : graph[i]) {
            matrix[i][to] = 1;
            matrix[to][i] = 1;
        }
    }

    std::vector<int> degree(city_count, 0);
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix.size(); ++j) {
            if (matrix[i][j] == 1) {
                ++degree[i];
            }
        }
    }

    for (int k=0; k < city_count; ++k)
        for (int i=0; i < city_count; ++i)
            for (int j=0; j < city_count; ++j)
                matrix[i][j] = std::min(matrix[i][j], matrix[i][k] + matrix[k][j]);

    std::set<int> lens;
    for (int i = 0; i < city_count; ++i) {
        for (int j = 0; j < city_count; ++j) {
            if (degree[i] == 1 && degree[j] == 1 && i != j) {
                lens.insert(matrix[i][j]);
            }
        }
    }
    std::vector<int> result;
    while(!lens.empty()) {
        result.push_back(*(lens.begin()));
        lens.erase(lens.begin());
    }
    return result;
}

void stressTest() {
    auto code = generatePrufer(6);
    printVector(code);
    auto graph = pruferDecode(code);
    printVector(graph);
    std::vector<int> lens = solve(6, graph);
    std::vector<int> easy_lens = easy_solve(6, graph);
    if (lens != easy_lens) {
        std::cerr << "KURWAAAAAAAAAAAAAAAAAAAA\n";
        printVector(lens);
        printVector(easy_lens);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    stressTest();
    return 0;
    int city_count;
    std::vector<std::vector<int>> graph;
    readInput(&city_count, &graph);
    std::vector<int> result = solve(city_count, graph);
    writeOutput(result);
}
