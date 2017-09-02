#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
#include <queue>
#include <ctime>

struct Request{

    char type;
    int left;
    int right;

    Request() {}

    Request(char _type, int _left, int _right) :
            type(_type),
            left(_left),
            right(_right) {}
};

void readInput(std::vector<Request>* requests,
               std::istream &in_stream = std::cin) {
    char type;
    int left;
    int right;
    while (in_stream >> type >> left >> right) {
        requests->push_back(Request(type, left, right));
    }
    requests->resize(requests->size());
}

void writeOutput(const std::vector<int>& result,
                 std::ostream &out_stream = std::cout) {
    for (int element : result) {
        std::cout << element << "\n";
    }
    std::cout << "\n";
}

class StaticMap{
public:
    explicit StaticMap(const std::vector<int>& ordered_vector) :
            ordered_data_(ordered_vector) {}

    int getIndex(int key) {
        return std::lower_bound(ordered_data_.begin(),
                                ordered_data_.end(),
                                key) - ordered_data_.begin();
    }

    int getSize() {
        return ordered_data_.size();
    }

private:
    std::vector<int> ordered_data_;
};

struct Maper{

    StaticMap lefts_map;
    StaticMap rights_map;

    Maper(StaticMap _lefts_map, StaticMap _rights_map) :
            lefts_map(_lefts_map),
            rights_map(_rights_map) {}
};

Maper getMaper(const std::vector<Request>& requests) {
    std::vector<int> lefts;
    lefts.reserve(requests.size());
    std::vector<int> rights;
    rights.reserve(requests.size());

    for (const Request& request : requests) {
        lefts.push_back(request.left);
        rights.push_back(request.right);
    }

    std::sort(lefts.begin(), lefts.end());
    std::sort(rights.begin(), rights.end());

    std::vector<int> set_lefts;
    std::vector<int> set_rights;

    for (int i = 0; i < requests.size(); ++i) {
        if (i == 0 || lefts[i] != lefts[i - 1]) {
            set_lefts.push_back(lefts[i]);
        }
        if (i == 0 || rights[i] != rights[i - 1]) {
            set_rights.push_back(rights[i]);
        }
    }

    return Maper(StaticMap(set_lefts), StaticMap(set_rights));
}

struct Vertex {

    int coord_x_left;
    int coord_x_right;

    int coord_y_left;
    int coord_y_right;
    int sum = 0;
    int ptrs[2][2];

    Vertex() {}

    Vertex(int _coord_x_left, int _coord_x_right, int _coord_y_left, int _coord_y_right) :
            coord_x_left(_coord_x_left),
            coord_x_right(_coord_x_right),
            coord_y_left(_coord_y_left),
            coord_y_right(_coord_y_right) {
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                ptrs[i][j] = -1;
            }
        }
    }

    void print() {
        std::cerr << "coordinates: ";
        std::cerr << coord_x_left << " "
                  << coord_x_right << " "
                  << coord_y_left << " "
                  << coord_y_right << "\n";
        std::cerr << "ptrs: ";
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                std::cerr << ptrs[i][j] << " ";
            }
        }
        std::cerr << "\nsum = " << sum << "\n";
    }
};

struct Rectangle {
    int left_x;
    int right_x;
    int left_y;
    int right_y;

    Rectangle(int x_left,
              int x_right,
              int y_left,
              int y_right,
              int move_x,
              int move_y) {
        int x_mid = (x_left + x_right) / 2;
        int y_mid = (y_left + y_right) / 2;
        if (move_x == 0) {
            left_x = x_left;
            right_x = x_mid;
        } else {
            left_x = x_mid;
            right_x = x_right;
        }
        if (move_y == 0) {
            left_y = y_left;
            right_y = y_mid;
        } else {
            left_y = y_mid;
            right_y = y_right;
        }
    }
};

class Tree2D {

public:
    explicit Tree2D(int side) {
        vertice[0] = Vertex(0, side, 0, side);
        ++size;
    }

    void print() {
        for (Vertex& vertex : vertice) {
            vertex.print();
        }
    }

    void add(int coord_x, int coord_y, int delta) {
        // std::cerr << "coords " << coord_x << " " << coord_y << "\n";
        int current_vertex = 0;
        bool flag_leaf = false;
        while (!flag_leaf) {
            vertice[current_vertex].sum += delta;
            int current_x_left = vertice[current_vertex].coord_x_left;
            int current_x_right = vertice[current_vertex].coord_x_right;
            int current_y_left = vertice[current_vertex].coord_y_left;
            int current_y_right = vertice[current_vertex].coord_y_right;
            if (current_x_right == current_x_left + 1 &&
                current_y_right == current_y_left + 1) {
                flag_leaf = true;
                // std::cerr << current_x_left << "current left x\n";
                // std::cerr << current_y_left << "current left y\n";
                // std::cerr << coord_x << " " << coord_y << "\n";
            } else {
                int current_x_mid = (current_x_left + current_x_right) / 2;
                int current_y_mid = (current_y_left + current_y_right) / 2;
                int x_move = 0;
                int y_move = 0;
                if (current_x_mid <= coord_x) {
                    x_move = 1;
                }
                if (current_y_mid <= coord_y) {
                    y_move = 1;
                }
                int next_vertex = vertice[current_vertex].ptrs[x_move][y_move];
                if (next_vertex == -1) {
                    vertice[current_vertex].ptrs[x_move][y_move] = size;
                    current_vertex = size;
                    Rectangle sub_rect = Rectangle(current_x_left,
                                                   current_x_right,
                                                   current_y_left,
                                                   current_y_right,
                                                   x_move,
                                                   y_move);
                    vertice[size] = Vertex(sub_rect.left_x,
                                             sub_rect.right_x,
                                             sub_rect.left_y,
                                             sub_rect.right_y);
                    ++size;
                } else {
                    current_vertex = next_vertex;
                }
            }
        }
    }

    int sum(int vertex, int left_x, int right_x, int left_y, int right_y) {

        if (vertex == -1) {
            return 0;
        }

        if (vertice[vertex].sum == 0) {
            return 0;
        }

        if (right_x <= left_x || right_y <= left_y) {
            return 0;
        }

        int current_x_left = vertice[vertex].coord_x_left;
        int current_x_right = vertice[vertex].coord_x_right;
        int current_y_left = vertice[vertex].coord_y_left;
        int current_y_right = vertice[vertex].coord_y_right;

        if (left_x == current_x_left &&
            right_x == current_x_right &&
            left_y == current_y_left &&
            right_y == current_y_right) {
            return vertice[vertex].sum;
        }

        int current_x_mid = (current_x_left + current_x_right) / 2;
        int current_y_mid = (current_y_left + current_y_right) / 2;

        int sums[2][2];

        sums[0][0] = sum(vertice[vertex].ptrs[0][0],
                         left_x,
                         std::min(current_x_mid, right_x),
                         left_y,
                         std::min(current_y_mid, right_y));

        sums[0][1] = sum(vertice[vertex].ptrs[0][1],
                         left_x,
                         std::min(current_x_mid, right_x),
                         std::max(current_y_mid, left_y),
                         right_y);

        sums[1][0] = sum(vertice[vertex].ptrs[1][0],
                         std::max(current_x_mid, left_x),
                         right_x,
                         left_y,
                         std::min(current_y_mid, right_y));

        sums[1][1] = sum(vertice[vertex].ptrs[1][1],
                         std::max(current_x_mid, left_x),
                         right_x,
                         std::max(current_y_mid, left_y),
                         right_y);

        int sum = 0;

        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                sum += sums[i][j];
            }
        }
        return sum;
    }

private:
    int size = 0;
    Vertex vertice[600000];
};

std::vector<int> solve(const std::vector<Request>& requests) {

    Maper maper = getMaper(requests);

    int maximal_value = -1;
    maximal_value = std::max(maximal_value, maper.lefts_map.getSize());
    maximal_value = std::max(maximal_value, maper.rights_map.getSize());
    Tree2D tree(maximal_value);

    std::vector<int> result;

    for (const Request& request : requests) {
        int left = maper.lefts_map.getIndex(request.left);
        int right = maper.rights_map.getIndex(request.right);
        if (request.type == '+') {
            int sum = tree.sum(0, left, maximal_value, 0, right + 1);
            result.push_back(sum);
            tree.add(left, right, 1);
        }
        if (request.type == '-') {
            int count = tree.sum(0, left, maximal_value, 0, right + 1);
            if (count > 0) {
                tree.add(left, right, -1);
            }}
    }
    return result;
}

/*
void generateTest(std::vector<Request>* requests) {
    int sz = 100000;
    int tests = 50000;
    for (int i = 0; i < tests; ++i) {
        int left = rand() % sz;
        int right = rand() % sz;
        if (left > right) {
            std::swap(left, right);
        }
        requests->push_back(Request('+', left, right));
    }
    std::random_shuffle(requests->begin(), requests->end());
}



int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::vector<Request> requests;
    // readInput(&requests);
    double avg = 0.0;
    for (int tests = 0; tests < 5; ++tests) {
        requests.clear();
        generateTest(&requests);
        time_t start = clock();
        std::vector<int> result = solve(requests);
        avg += (clock() - start) / 1.0 / CLOCKS_PER_SEC;
        std::cout << (clock() - start) / 1.0 / CLOCKS_PER_SEC << "\n";
        std::cout << result.size() << "\n";
    }
    std::cout << "avg = " << avg / 5 << "\n";
    // writeOutput(result);
}
*/
/*

void StressTest() {

    int sz = 50;
    int requests = 500;
    int mod = 200;

    std::vector<std::vector<int>> grid(sz, std::vector<int> (sz, 0));
    Tree2D tree(sz);

    for (int request = 0; request < requests; ++request) {
        int coord_x = rand() % sz;
        int coord_y = rand() % sz;
        int delta = 1;
        // std::cerr << "coord_x = " << coord_x << ", coord_y = " << coord_y << ", delta = " << delta << "\n";
        grid[coord_x][coord_y] += delta;
        tree.add(coord_x, coord_y, delta);

        for (int l_x = 0; l_x < sz; ++l_x) {
            for (int r_x = l_x + 1; r_x < sz; ++r_x) {
                for (int l_y = 0; l_y < sz; ++l_y) {
                    for (int r_y = l_y + 1; r_y < sz; ++r_y) {

                        int sum_brute = 0;
                        for (int x = l_x; x < r_x; ++x) {
                            for (int y = l_y; y < r_y; ++y) {
                                sum_brute += grid[x][y];
                            }
                        }
                        // std::cerr << "brute: " << sum_brute << "\n";

                        // std::cerr << "new requesttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt\n";

                        int sum_clev = tree.sum(0, l_x, r_x, l_y, r_y);
                        // std::cerr << "clev: " << sum_clev << "\n";
                        if (sum_brute != sum_clev) {
                            std::cerr << "KURWAaAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n";
                            std::cerr << "ask sum on " << l_x << " " << r_x << " | " << l_y << " " << r_y << "\n";
                            std::cerr << "brute = " << sum_brute << "\n";
                            std::cerr << "clev = " << sum_clev << "\n";
                            // int a = (3) / (sum_brute - sum_brute);
                            std::terminate();
                        }
                    }
                }
            }
        }
        std::cerr << "OK\n";

    }
}
*/

int main() {
    // StressTest();
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::vector<Request> requests;
    readInput(&requests);
    std::vector<int> result = solve(requests);
    writeOutput(result);
}
