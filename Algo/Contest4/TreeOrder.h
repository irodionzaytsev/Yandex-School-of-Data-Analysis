#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
#include <list>
#include <ctime>

const int MAX = 1000000001;

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
}

void readInput(std::vector<int>* pre_order,
               std::istream &in_stream = std::cin) {
    int vertex_count;
    in_stream >> vertex_count;
    pre_order->reserve(vertex_count);
    for (int i = 0; i < vertex_count; ++i) {
        int key;
        in_stream >> key;
        pre_order->push_back(key);
    }
}

void writeOutput(const std::vector<int>& post_order,
                 const std::vector<int>& in_order,
                 std::ostream &out_stream = std::cout) {
    for (int post : post_order) {
        std::cout << post << " ";
    }
    std::cout << "\n";
    for (int in : in_order) {
        std::cout << in << " ";
    }
    std::cout << "\n";
}

struct Result{
    std::vector<int> post_order;
    std::vector<int> in_order;
};

struct Vertex{
    Vertex* left_ptr;
    Vertex* right_ptr;
    int key;
    int size = 1;

    Vertex(Vertex* _left_ptr,
           Vertex* _right_ptr,
           int _key) :
            left_ptr(_left_ptr),
            right_ptr(_right_ptr),
            key(_key) {}
};

class BinaryTree {

public:
    explicit BinaryTree(const std::vector<int>& pre_order) {

        head = new Vertex{nullptr, nullptr, pre_order[0]};

        std::vector<Vertex*> open_rights;
        open_rights.push_back(head);
        Vertex* last_vertex = head;

        for (int add = 1; add < pre_order.size(); ++add) {
            Vertex* add_vertex = new Vertex{nullptr, nullptr, pre_order[add]};
            if (last_vertex->key > add_vertex->key) {
                last_vertex->left_ptr = add_vertex;
                open_rights.push_back(add_vertex);
                last_vertex = add_vertex;
            } else {

                while (open_rights.size() > 0 && open_rights.back()->key <= add_vertex->key) {
                    last_vertex = open_rights.back();
                    open_rights.pop_back();
                }
                last_vertex->right_ptr = add_vertex;
                open_rights.push_back(add_vertex);
                last_vertex = add_vertex;
            }
        }
        initSizes(head);
    }

    void getInOrder(const std::vector<int>::iterator out_first,
                    const std::vector<int>::iterator out_last,
                    Vertex* vertex) {
        if (vertex == nullptr) {
            return;
        }
        int left_size = 0;
        int right_size = 0;
        if (vertex->left_ptr != nullptr) {
            left_size = vertex->left_ptr->size;
        }
        if (vertex->right_ptr != nullptr) {
            right_size = vertex->right_ptr->size;
        }
        getInOrder(out_first, out_first + left_size, vertex->left_ptr);
        *(out_first + left_size) = vertex->key;
        getInOrder(out_first + left_size + 1, out_last, vertex->right_ptr);
    }

    void getInOrder(const std::vector<int>::iterator out_first,
                    const std::vector<int>::iterator out_last) {
        getInOrder(out_first, out_last, head);
    }

    void getPostOrder(const std::vector<int>::iterator out_first,
                      const std::vector<int>::iterator out_last,
                      Vertex* vertex) {
        if (vertex == nullptr) {
            return;
        }
        int left_size = 0;
        int right_size = 0;
        if (vertex->left_ptr != nullptr) {
            left_size = vertex->left_ptr->size;
        }
        if (vertex->right_ptr != nullptr) {
            right_size = vertex->right_ptr->size;
        }
        getPostOrder(out_first, out_first + left_size, vertex->left_ptr);
        getPostOrder(out_first + left_size, out_last + left_size + right_size, vertex->right_ptr);
        *(out_first + left_size + right_size) = vertex->key;
    }

    void getPostOrder(const std::vector<int>::iterator out_first,
                    const std::vector<int>::iterator out_last) {
        getPostOrder(out_first, out_last, head);
    }

    void clear(Vertex* vertex) {
        if (vertex->left_ptr != nullptr) {
            clear(vertex->left_ptr);
        }
        if (vertex->right_ptr != nullptr) {
            clear(vertex->right_ptr);
        }
        delete vertex;
    }

    void clear() {
        clear(head);
    }

private:
    Vertex* head;

    void initSizes(Vertex* vertex) {
        if (vertex->left_ptr != nullptr) {
            initSizes(vertex->left_ptr);
            vertex->size += vertex->left_ptr->size;
        }
        if (vertex->right_ptr != nullptr) {
            initSizes(vertex->right_ptr);
            vertex->size += vertex->right_ptr->size;
        }
    }
};

Result solve(const std::vector<int>& pre_order) {
    if (pre_order.size() == 0) {
        return Result{std::vector<int>(), std::vector<int>()};
    }
    BinaryTree binaryTree(pre_order);
    std::vector<int> in_order(pre_order.size());
    std::vector<int> post_order(pre_order.size());
    binaryTree.getInOrder(in_order.begin(), in_order.end());
    binaryTree.getPostOrder(post_order.begin(), post_order.end());
    return Result{post_order, in_order};
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::vector<int> pre_order;
    readInput(&pre_order);
    Result result = solve(pre_order);
    writeOutput(result.post_order, result.in_order);
}
