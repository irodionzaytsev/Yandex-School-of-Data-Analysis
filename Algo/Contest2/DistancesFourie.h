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
#include <complex>
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

void writeOutput(const std::vector<int>& result, std::ostream &out_stream = std::cout) {
    out_stream << result.size() << "\n";
    for (int dist : result) {
        out_stream << dist << "\n";
    }
}

std::vector<int64_t> makeUnion(const std::vector<std::vector<bool>>& sets) {
    size_t max_size = 0;
    for (int i = 0; i < sets.size(); ++i) {
        max_size = std::max(max_size, sets[i].size());
    }
    std::vector<int64_t> sets_union(max_size, 0);
    for (int i = 0; i < sets.size(); ++i) {
        for (int j = 0; j < sets[i].size(); ++j) {
            if (sets[i][j]) {
                ++sets_union[j];
            }
        }
    }
    return sets_union;
}

// fft is taken from e-maxx algo
const long double PI = 3.1415926;
typedef std::complex<double> base;

int rev (int num, int lg_n) {
    int res = 0;
    for (int i = 0; i < lg_n; ++i) {
        if (num & (1 << i)) {
            res |= 1 << (lg_n - 1 - i);
        }
    }
    return res;
}

void fft (std::vector<base>& set_a, bool invert) {
    int size_a = set_a.size();
    int lg_n = 0;
    while ((1 << lg_n) < size_a) {
        ++lg_n;
    }

    for (int i = 0; i < size_a; ++i) {
        if (i < rev(i, lg_n)) {
            std::swap(set_a[i], set_a[rev(i, lg_n)]);
        }
    }

    for (int len = 2; len <= size_a; len <<= 1) {
        double ang = 2*PI/len * (invert ? -1 : 1);
        base wlen (cos(ang), sin(ang));
        for (int i = 0; i < size_a; i += len) {
            base w(1);
            for (int j = 0; j < len / 2; ++j) {
                base u = set_a[i + j];
                base v = set_a[i + j + len / 2] * w;
                set_a[i + j] = u + v;
                set_a[i + j + len/2] = u - v;
                w *= wlen;
            }
        }
    }
    if (invert) {
        for (int i = 0; i < size_a; ++i) {
            set_a[i] /= size_a;
        }
    }
}

std::vector<int64_t> multiply (const std::vector<int64_t>& set_a,
                               const std::vector<int64_t>& set_b) {
    std::vector<base> fourie_a (set_a.begin(), set_a.end()),  fourie_b (set_b.begin(), set_b.end());
    size_t degree = 1;
    while (degree < std::max (set_a.size(), set_b.size())) {
        degree <<= 1;
    }
    degree <<= 1;
    fourie_a.resize (degree),  fourie_b.resize (degree);

    fft (fourie_a, false);
    fft (fourie_b, false);

    for (size_t i = 0; i < degree; ++i) {
        fourie_a[i] *= fourie_b[i];
    }
    fft (fourie_a, true);

    std::vector<int64_t> res(degree);

    for (size_t i = 0; i < degree; ++i) {
        res[i] = int64_t(fourie_a[i].real() + 0.5);
    }
    return res;
}


std::vector<int64_t> sum(const std::vector<int64_t>& set_a, const std::vector<int64_t>& set_b) {
    std::vector<int64_t> set_c (set_a.size() + set_b.size(), 0);
    for (int i = 0; i < set_a.size(); ++i) {
        for (int j = 0; j < set_b.size(); ++j) {
            set_c[i + j] += set_a[i] * set_b[j];
        }
    }
    return set_c;
}

std::vector<int64_t> sumFourie(const std::vector<int64_t>& set_a,
                               const std::vector<int64_t>& set_b) {
    // return std::vector<int64_t> (set_a.size() + set_b.size(), 0);
    if (set_a.size() < 100 && set_b.size() < 100) {
        return sum(set_a, set_b);
    }

    std::vector<int64_t> set_c = multiply(set_a, set_b);
    return set_c;
}

void decrement(std::vector<int64_t>* set_a, std::vector<int64_t> set_b) {
    for (int i = 0; i < set_b.size(); ++i) {
        (*set_a)[i] -= set_b[i];
    }
}

std::vector<int64_t> setToInt(const std::vector<bool>& set) {
    std::vector<int64_t> set_int(set.size(), 0);
    for (int i = 0; i < set.size(); ++i) {
        if (set[i]) {
            ++set_int[i];
        }
    }
    return set_int;
}

std::vector<bool> setToBool(const std::vector<int64_t>& set) {
    std::vector<bool> set_bool(set.size(), 0);
    for (int i = 0; i < set.size(); ++i) {
        if (set[i] > 0) {
            set_bool[i] = true;
        }
    }
    return set_bool;
}

std::vector<bool> fastSum(const std::vector<std::vector<bool>>& sets) {
    std::vector<int64_t> sets_union = makeUnion(sets);
    // std::vector<int64_t> union_sum = sum(sets_union, sets_union);
    std::vector<int64_t> union_sum = sumFourie(sets_union, sets_union);
    for (int i = 0; i < sets.size(); ++i) {
        // decrement(&union_sum, sum(setToInt(sets[i]), setToInt(sets[i])));
        decrement(&union_sum, sumFourie(setToInt(sets[i]), setToInt(sets[i])));
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

class Vertex {
private:
    std::vector<Vertex*> adjacents_;
    int sub_size_;
    bool is_leaf_;
    Vertex* parent_;

public:
    Vertex() {
        sub_size_ = 1;
    };

    void killParent() {
        for (int i = 0; i < adjacents_.size(); ++i) {
            if (adjacents_[i] == parent_) {
                std::swap(adjacents_[i], adjacents_[adjacents_.size() - 1]);
                adjacents_.pop_back();
                parent_ = nullptr;
            }
        }
    }

    void setParent(Vertex* parent) {
        this->parent_ = parent;
        adjacents_.push_back(parent_);
    }

    int getSubSize() const {
        return sub_size_;
    }

    bool isLeaf() const {
        return is_leaf_;
    }

    void dfs_init(const int vertex,
                  const std::vector<std::vector<int>>& graph,
                  const int parent = -1) {
        for (int to : graph[vertex]) {
            if (to == parent) {
                continue;
            }
            adjacents_.push_back(new Vertex());
            adjacents_.back()->setParent(this);
            adjacents_.back()->dfs_init(to, graph, vertex);
            sub_size_ += adjacents_.back()->getSubSize();
        }

        if (adjacents_.size() == 1 || (adjacents_.size() == 0 && parent_ == nullptr)) {
            is_leaf_ = true;
        } else {
            is_leaf_ = false;
        }
    }

    Vertex* getCentroid(const int& graph_size) {
        for (Vertex* child : adjacents_) {
            if (child != parent_ && 2 * child->getSubSize() >= graph_size) {
                return child->getCentroid(graph_size);
            }
        }
        return this;
    }

    void reRoot(Vertex* parent) {
        parent_ = parent;
        sub_size_ = 1;
        for (Vertex* child : adjacents_) {
            if (child == parent_) {
                continue;
            }
            child->reRoot(this);
            sub_size_ += child->getSubSize();
        }
    }

    void dfs_path_lens(std::vector<bool>* bool_set_path_lens, int current_len) const {
        if (parent_ == nullptr && is_leaf_ && adjacents_.size() == 0) {
            bool_set_path_lens->push_back(false);
            bool_set_path_lens->push_back(true);
            return;
        }
        if (is_leaf_ && current_len > 0) {
            while (bool_set_path_lens->size() <= current_len + 1) {
                bool_set_path_lens->push_back(false);
            }
            (*bool_set_path_lens)[current_len + 1] = true;
        } else {
            for (Vertex* child : adjacents_) {
                if (child != parent_) {
                    child->dfs_path_lens(bool_set_path_lens, current_len + 1);
                }
            }
        }
    }

    std::vector<Vertex*> getAdjacents() {
        return adjacents_;
    }

    void print(const std::string& translation_s, std::ostream &os = std::cerr) {
        os << "\n" << translation_s << "START\n";
        os << translation_s << "adrress: " << this << "\n";
        os << translation_s << "sub_size_: " << sub_size_ << "\n";
        os << translation_s << "is_leaf_: " << is_leaf_ << "\n";
        os << translation_s << "children:\n";
        os << translation_s;
        for (Vertex* child : adjacents_) {
            if (child != parent_) {
                os << child << " ";
            }
        }
        os << "\n";
        os << translation_s << "FINISH\n";
        for (Vertex* child : adjacents_) {
            if (child != parent_) {
                child->print(translation_s, os);
            }
        }
    }

    void clear() {
        for (Vertex* child : adjacents_) {
            if (child != parent_) {
                child->clear();
            }
        }
        delete this;
    }
};


class Graph {

private:
    Vertex* root_;
    int graph_size_;

public:
    explicit Graph(const std::vector<std::vector<int>>& graph) {
        root_ = new Vertex();
        graph_size_ = graph.size();
        root_->dfs_init(0, graph);
    }

    explicit Graph(Vertex* root) {
        root_ = root;
        graph_size_ = root->getSubSize();
    }

    void makeCentroidal() {
        Vertex* centroid = root_->getCentroid(graph_size_);
        root_ = centroid;
        centroid->reRoot(nullptr);
    }

    std::vector<bool> getPathLens() const {
        std::vector<bool> bool_set_path_lens;
        root_->dfs_path_lens(&bool_set_path_lens, 0);
        return bool_set_path_lens;
    }

    std::vector<bool> getLens() const {
        if (graph_size_ == 1) {
            return std::vector<bool>();
        }
        std::vector<std::vector<bool>> set_lens;
        std::vector<std::vector<bool>> set_paths;
        for (Vertex* child : root_->getAdjacents()) {
            child->killParent();
            Graph sub_graph(child);
            set_paths.push_back(sub_graph.getPathLens());
            sub_graph.makeCentroidal();
            set_lens.push_back(sub_graph.getLens());
            // child->setParent(root_);
        }
        // root_->reRoot(nullptr);
        std::vector<bool> sum_sub_path_lens = fastSum(set_paths);
        set_lens.push_back(sum_sub_path_lens);
        std::vector<bool> lens = concatenateBoolSets(set_lens);
        return lens;
    }

    void print(const int translation, bool print_mod, std::ostream &os = std::cerr) {
        std::string translation_s = "";
        for (int i = 0; i < translation; ++i) {
            translation_s.push_back(' ');
        }
        os << translation_s << "the graph describing:\n";

        root_->print(translation_s);

        if (print_mod) {
            os << translation_s << "path lens:\n";
            std::vector<bool> pathLens = getPathLens();
            os << translation_s;
            for (int i = 0; i < pathLens.size(); ++i) {

                if (pathLens[i]) {
                    os << i << " ";
                }
            }
            os << translation_s << "\n";
            os << translation_s << "leafs distances:\n";
            std::vector<bool> dists = getLens();
            os << "counted\n";
            // os << translation_s << "dists.size() = " << dists.size() << "\n";
            os << translation_s;
            for (int i = 0; i < dists.size(); ++i) {
                if (dists[i]) {
                    os << i << " ";
                }
            }
        }
        os << translation_s << "\n";
    }

    void clear() {
        root_->clear();
    }
};


std::vector<int> solve(int city_count, std::vector<std::vector<int>> edges) {
    if (city_count == 2) {
        return std::vector<int>({1});
    }
    Graph graph = Graph(edges);
    graph.makeCentroidal();
    std::vector<bool> lens = graph.getLens();
    graph.clear();
    std::vector<int> result;
    for (int len = 0; len < lens.size(); ++len) {
        if (lens[len]) {
            result.push_back(len);
        }
    }
    return result;
}


/*
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

std::vector<std::vector<int>> makeLinesGraph(int lines, int len) {

    std::vector<std::vector<int>> graph(1);
    for(int i = 1; i <= lines; ++i) {
        graph.push_back(std::vector<int>());
        graph[0].push_back(i);
        graph.back().push_back(0);
    }
    for (int i = 2; i <= len; ++i) {
        for (int j = 1; j <= lines; ++j) {
            int vertex = (i - 1) * lines + j;
            graph[vertex - lines].push_back(vertex);
            graph.push_back(std::vector<int>());
            graph.back().push_back(vertex - lines);
        }
    }
    return graph;
}

void lineTest() {
    int lines = 10;
    int len = 20000;
    auto graph = makeLinesGraph(lines, len);
    // printVector(graph);
    // std::cerr << "heh\n";
    clock_t start = clock();
    std::vector<int> lens = solve(lines * len + 1, graph);
    printVector(lens);
    std::cerr << (clock() - start) / 1.0 / CLOCKS_PER_SEC << "\n";
}

void stressTest() {
    int sz = 200;
    for (int test = 0; test < 10; ++test) {
        std::cerr << "test = " << test << "\n";
        auto code = generatePrufer(sz);

        auto graph = pruferDecode(code);

        clock_t start = clock();
        std::vector<int> lens = solve(sz, graph);
        std::cerr << (clock() - start) / 1.0 / CLOCKS_PER_SEC << "\n";

        std::vector<int> easy_lens = easy_solve(sz, graph);

        if (lens != easy_lens) {
            std::cerr << "KURWAAAAAAAAAAAAAAAAAAAA\n";
            printVector(code);
            printVector(graph);
            printVector(lens);
            printVector(easy_lens);
        } else {
            std::cerr << "OK\n";
        }

    }


}
*/


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    // lineTest();
    // stressTest();
    // return 0;
    int city_count;
    std::vector<std::vector<int>> graph;
    readInput(&city_count, &graph);
    std::vector<int> result = solve(city_count, graph);
    writeOutput(result);
}


