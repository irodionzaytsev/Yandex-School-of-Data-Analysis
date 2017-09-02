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
    StaticMap() {}

    explicit StaticMap(const std::vector<int>::iterator first,
                       const std::vector<int>::iterator last) {
        std::vector<int> ordered_copy;
        for (auto it = first; it != last; ++it) {
            ordered_copy.push_back(*it);
        }
        std::sort(ordered_copy.begin(), ordered_copy.end());
        for (int i = 0; i < ordered_copy.size(); ++i) {
            if (i == 0 || ordered_copy[i] != ordered_copy[i - 1]) {
                ordered_data_.push_back(ordered_copy[i]);
            }
        }
        ordered_data_.resize(ordered_data_.size());
    }

    explicit StaticMap(const std::vector<std::pair<int, int>>::iterator first,
                       const std::vector<std::pair<int, int>>::iterator last) {
        std::vector<int> ordered_copy;
        for (auto it = first; it != last; ++it) {
            ordered_copy.push_back(it->second);
        }
        std::sort(ordered_copy.begin(), ordered_copy.end());
        for (int i = 0; i < ordered_copy.size(); ++i) {
            if (i == 0 || ordered_copy[i] != ordered_copy[i - 1]) {
                ordered_data_.push_back(ordered_copy[i]);
            }
        }
        ordered_data_.resize(ordered_data_.size());
    }

    explicit StaticMap(const std::vector<std::pair<int, int>>::const_iterator first,
                       const std::vector<std::pair<int, int>>::const_iterator last) {
        std::vector<int> ordered_copy;
        for (auto it = first; it != last; ++it) {
            ordered_copy.push_back(it->second);
        }
        std::sort(ordered_copy.begin(), ordered_copy.end());
        for (int i = 0; i < ordered_copy.size(); ++i) {
            if (i == 0 || ordered_copy[i] != ordered_copy[i - 1]) {
                ordered_data_.push_back(ordered_copy[i]);
            }
        }
        ordered_data_.resize(ordered_data_.size());
    }

    int getIndex(int key) {
        return std::lower_bound(ordered_data_.begin(),
                                ordered_data_.end(),
                                key) - ordered_data_.begin();
    }

    int getSize() {
        return ordered_data_.size();
    }

    std::vector<int> getData() {
        return ordered_data_;
    }

private:
    std::vector<int> ordered_data_;
};


class Fenwick {
private:
    std::vector<int> indicators_;
    int len_;

    int sum(int index) {
        int result = 0;
        for (; index >= 0; index = (index & (index + 1)) - 1)
            result += indicators_[index];
        return result;
    }

public:
    Fenwick() {}

    void initialize(int _len) {
        len_ = _len;
        indicators_.assign(len_, 0);
    }

    void add(int index, int delta) {
        for (; index < len_; index = (index | (index + 1)))
            indicators_[index] += delta;
    }

    int sum(int left, int right) {
        return sum(right - 1) - sum(left - 1);
    }
};


struct Vertex {

    int left;
    int right;
    int ptrs[2];
    StaticMap y_keys;
    Fenwick fenwick;

    Vertex() {}

    Vertex(int left_x,
           int right_x,
           const std::vector<std::pair<int, int>>::const_iterator first,
           const std::vector<std::pair<int, int>>::const_iterator last) {
        left = left_x;
        right = right_x;
        ptrs[0] = -1;
        ptrs[1] = -1;
        y_keys = StaticMap(first, last);

        /*
        std::cerr << "this static map\n";
        for (int i = 0; i < y_keys.getData().size(); ++i) {
            std::cerr << y_keys.getData()[i] << " ";
        }
        std::cerr << "\n";
        */
        fenwick.initialize(y_keys.getSize());
    }

    void print() {
        std::cerr << "coordinates: ";
        std::cerr << left << " " << right << "\n";
        std::cerr << "ptrs: ";
        for (int i = 0; i < 2; ++i) {
                std::cerr << ptrs[i] << " ";
        }
        std::cerr << "\n";
    }
};

class SegmentFenwick{

public:
    explicit SegmentFenwick(const std::vector<std::pair<int, int>>& xy_keys) {

        std::vector<int> vector_x_keys;
        for (auto xy_key : xy_keys) {
            vector_x_keys.push_back(xy_key.first);
        }
        x_keys = StaticMap(vector_x_keys.begin(), vector_x_keys.end());
        int x_level  = x_keys.getSize();
        std::vector<std::pair<int, int>> xy_keys_copy = xy_keys;
        for (int i = 0; i < xy_keys_copy.size(); ++i) {
            xy_keys_copy[i].first = x_keys.getIndex(xy_keys_copy[i].first);
        }
        init(0, x_level, xy_keys_copy.begin(), xy_keys_copy.end());
        // for (Vertex& vertex : vertice) {
        //     vertex.print();
        // }
    }

    int init(int left,
             int right,
             const std::vector<std::pair<int, int>>::const_iterator first,
             const std::vector<std::pair<int, int>>::const_iterator last) {
        /*
        std::cerr << "init left right\n";
        std::cerr << left << " " << right << "\n";
        std::cerr << "first, last\n";
        std::cerr << first->first << " " << first->second << " | " << (last - 1)->first << " " << (last - 1)->second << "\n";
        */
        int vertex_number = vertice.size();
        vertice.push_back(Vertex(left, right, first, last));
        if (right > left + 1) {
            int mid = (left + right) / 2;
            int mid_it = upper_bound(first, last, std::make_pair(mid, -1000000009)) - first;
            int left_son = init(left, mid, first, first + mid_it);
            int right_son = init(mid, right, first + mid_it, last);
            vertice[vertex_number].ptrs[0] = left_son;
            vertice[vertex_number].ptrs[1] = right_son;
        }
        return vertex_number;
    }

    void add(int current_vertex, int coord_x, int coord_y, int delta) {

        if (current_vertex == 0) {
            coord_x = x_keys.getIndex(coord_x);
        }
        /*
        std::cerr << "current_vertex coord_x coord_y\n";
        std::cerr << current_vertex << " " << coord_x << " " << coord_y << "\n";
        std::cerr << vertice[current_vertex].left << " | " << vertice[current_vertex].right << "\n";
        */
        int compressed_coord_y = vertice[current_vertex].y_keys.getIndex(coord_y);
        // std::cerr << "compressed_y = " << compressed_coord_y << "\n";
        vertice[current_vertex].fenwick.add(compressed_coord_y, delta);
        if (vertice[current_vertex].right > vertice[current_vertex].left + 1) {
            int mid = (vertice[current_vertex].left + vertice[current_vertex].right) / 2;
            if (coord_x < mid) {
                add(vertice[current_vertex].ptrs[0], coord_x, coord_y, delta);
            } else {
                add(vertice[current_vertex].ptrs[1], coord_x, coord_y, delta);
            }
        }
    }

    int sum(int current_vertex, int left_x, int right_x, int left_y, int right_y) {

        if (current_vertex == 0) {
            left_x = x_keys.getIndex(left_x);
            right_x = x_keys.getIndex(right_x);
        }

        if (left_x >= right_x || current_vertex == -1) {
            return 0;
        }
        if (vertice[current_vertex].left == left_x &&
            vertice[current_vertex].right == right_x) {
            int compressed_left_y = vertice[current_vertex].y_keys.getIndex((left_y));
            int compressed_right_y = vertice[current_vertex].y_keys.getIndex((right_y));
            return vertice[current_vertex].fenwick.sum(compressed_left_y, compressed_right_y);
        }
        int mid = (vertice[current_vertex].left + vertice[current_vertex].right) / 2;
        int first_ptr = vertice[current_vertex].ptrs[0];
        int second_ptr = vertice[current_vertex].ptrs[1];
        return sum(first_ptr, left_x, std::min(mid, right_x), left_y, right_y) +
               sum(second_ptr, std::max(mid, left_x), right_x, left_y, right_y);
    }

    int getSize() {
        return x_keys.getSize();
    }

private:
    StaticMap x_keys;
    std::vector<Vertex> vertice;
};

std::vector<int> solve(const std::vector<Request>& requests) {


    std::vector<std::pair<int, int>> xy_keys;
    xy_keys.reserve(requests.size());
    for (const Request& request : requests) {
        xy_keys.push_back(std::make_pair(request.left, request.right));
    }
    std::sort(xy_keys.begin(), xy_keys.end());
    SegmentFenwick segmentFenwick(xy_keys);
    int x_level = xy_keys.back().first + 1;
    std::vector<int> result;

    for (const Request& request : requests) {
        int left = request.left;
        int right = request.right;
        if (request.type == '+') {
            int sum = segmentFenwick.sum(0, left, x_level, 0, right + 1);
            result.push_back(sum);
            segmentFenwick.add(0, left, right, 1);
        }
        if (request.type == '-') {
            int count = segmentFenwick.sum(0, left, x_level, 0, right + 1);
            if (count > 0) {
                segmentFenwick.add(0, left, right, -1);
            }}
    }
    return result;
}

int main() {
    // StressTest();
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::vector<Request> requests;
    readInput(&requests);
    std::vector<int> result = solve(requests);
    writeOutput(result);
}
