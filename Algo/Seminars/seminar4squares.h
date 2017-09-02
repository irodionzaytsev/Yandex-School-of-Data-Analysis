#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
#include <queue>

const int SMALLCONST = -1000000001;
const int mod = 2147483647;

void readInput(int* width,
               int* height,
               std::vector<std::vector<int>>* grid,
               std::istream &in_stream = std::cin) {
    in_stream >> *width;
    in_stream >> *height;
    grid->assign(*width, std::vector<int>(*height));
    char character;
    for (int i = 0; i < *width; ++i) {
        for (int j = 0; j < *height; ++j) {
            in_stream >> character;
            (*grid)[i][j] = character - 'a';
        }
    }
}

struct Result {
    int size;
    int left_x, left_y;
    int right_x, right_y;
};

void writeOutput(const Result& result, std::ostream &out_stream = std::cout) {
    out_stream << result.size << "\n";
    if (result.size > 0) {
        out_stream << result.left_x << " " << result.left_y << "\n";
        out_stream << result.right_x << " " << result.right_y << "\n";
    }
}

class Hasher {
public:
    Hasher(const int width,
           const int height,
           const std::vector<std::vector<int>>& grid,
           const long long base_p,
           const long long base_q,
           const long long prime) :
            width_(width),
            height_(height),
            base_p_(base_p),
            base_q_(base_q),
            prime_(prime) {
        make_powers_(&p_powers, base_p_, width + 1);
        make_powers_(&q_powers, base_q_, height + 1);
        rectangle_sums_.assign(width, std::vector<long long> (height, 0));
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                rectangle_sums_[i][j] = (rectangle_sum_(i - 1, j) +
                                        rectangle_sum_(i, j - 1) -
                                        rectangle_sum_(i - 1, j - 1) +
                                        2 * prime_) % prime_;
                long long grid_value = (p_powers[i] * q_powers[j]) % prime_;
                grid_value = (grid_value * (long long)grid[i][j]) % prime_;
                rectangle_sums_[i][j] = (rectangle_sums_[i][j] + grid_value) % prime_;
            }
        }
    };

    long long getHash(int left_x, int left_y, int right_x, int right_y) const {
        long long hash = rectangle_sum_(right_x - 1, right_y - 1) -
                         rectangle_sum_(left_x - 1, right_y - 1) -
                         rectangle_sum_(right_x - 1, left_y - 1) +
                         rectangle_sum_(left_x - 1, left_y - 1) + 3 * prime_;
        hash %= prime_;
        long long mult = (p_powers[width_ - left_x] * q_powers[height_ - left_y]) % prime_;
        return (hash * mult) % prime_;
    }

private:
    int width_;
    int height_;
    long long base_p_;
    long long base_q_;
    long long prime_;
    std::vector<long long> p_powers;
    std::vector<long long> q_powers;
    std::vector<std::vector<long long>> rectangle_sums_;
    void make_powers_(std::vector<long long>* powers, long long base, int len) {
        long long power = 1ll;
        for (int i = 0; i < len; ++i) {
            (*powers).push_back(power);
            power = (power * base) % prime_;
        }
    }

    long long rectangle_sum_(int index_i, int index_j) const {
        if (index_i < 0 || index_j < 0) {
            return 0;
        }
        return rectangle_sums_[index_i][index_j];
    }
};

struct HashCell {
    long long hash_f;
    long long hash_s;
    int index_x;
    int index_y;

    HashCell(long long _hash_f,
             long long _hash_s,
             int _index_x,
             int _index_y) :
            hash_f(_hash_f),
            hash_s(_hash_s),
            index_x(_index_x),
            index_y(_index_y) {};

    bool operator < (const HashCell& other) const {
        if (this->hash_f < other.hash_f) {
            return true;
        }
        if (this->hash_f > other.hash_f) {
            return false;
        }
        return (this->hash_s < other.hash_s);
    }

    bool operator == (const HashCell& other) const {
        return (this->hash_f == other.hash_f && this->hash_s == other.hash_s);
    }
};

bool check_for_square(const int width,
                      const int height,
                      const int size,
                      const Hasher& hasher_f,
                      const Hasher& hasher_s,
                      int* left_x,
                      int* left_y,
                      int* right_x,
                      int* right_y) {

    std::vector<HashCell> hash_cells;
    for (int index_x = 0; index_x + size <= width; ++index_x) {
        for (int index_y = 0; index_y + size <= height; ++index_y) {
            hash_cells.push_back(
                    HashCell(hasher_f.getHash(index_x, index_y, index_x + size, index_y + size),
                             hasher_s.getHash(index_x, index_y, index_x + size, index_y + size),
                             index_x,
                             index_y));
        }
    }

    std::sort(hash_cells.begin(), hash_cells.end());
    for (int i = 0; i + 1 < hash_cells.size(); ++i) {
        if (hash_cells[i] == hash_cells[i + 1]) {
            *left_x = hash_cells[i].index_x;
            *left_y = hash_cells[i].index_y;
            *right_x = hash_cells[i + 1].index_x;
            *right_y = hash_cells[i + 1].index_y;
            return true;
        }
    }
    return false;
}

Result solve(const int width,
             const int height,
             std::vector<std::vector<int>>* grid) {

    Hasher hasher_f(width, height, *grid, 50, 51, 1000000007);
    Hasher hasher_s(width, height, *grid, 228, 1488, 1000000009);
    (*grid).clear();


    int left_pointer = 0;
    int right_pointer = std::min(width, height) + 1;
    int left_x;
    int left_y;
    int right_x;
    int right_y;
    while (right_pointer - left_pointer > 1) {
        int mid_pointer = (left_pointer + right_pointer) / 2;
        if (check_for_square(width,
                             height,
                             mid_pointer,
                             hasher_f,
                             hasher_s,
                             &left_x,
                             &left_y,
                             &right_x,
                             &right_y)) {
            left_pointer = mid_pointer;
        } else {
            right_pointer = mid_pointer;
        }
    }
    Result result;
    result.size = left_pointer;
    result.left_x = left_x + 1;
    result.left_y = left_y + 1;
    result.right_x = right_x + 1;
    result.right_y = right_y + 1;
    return result;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    int width;
    int height;
    std::vector<std::vector<int>> grid;
    readInput(&width, &height, &grid);
    Result result = solve(width, height, &grid);
    writeOutput(result);
}
