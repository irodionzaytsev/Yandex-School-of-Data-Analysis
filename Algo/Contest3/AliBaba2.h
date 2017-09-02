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
}

struct Coin {

    int place;
    int value;

    Coin (int _place, int _value) :
            place(_place),
            value(_value) {};

    friend std::ostream& operator<< (std::ostream& os, const Coin& obj) {
        os << obj.place << " -> " << obj.value << "\n";
        return os;
    }

    bool operator < (const Coin& other_coin) {
        return this->place < other_coin.place;
    }
};

void readInput(size_t* coin_count,
               std::vector<Coin>* coins,
               std::istream &in_stream = std::cin) {
    in_stream >> *coin_count;
    int place;
    int value;
    for (int i = 0; i < *coin_count; ++i) {
        in_stream >> place >> value;
        coins->push_back(Coin(place, value));
    }
    std::sort(coins->begin(), coins->end());
    // printVector(*coins);
}


void writeOutput(int result, std::ostream &out_stream = std::cout) {
    if (result <= 1000000) {
        out_stream << result << "\n";
    } else {
        out_stream << "No solution\n";
    }
}

int getDist(const std::vector<Coin>& coins, int left, int right) {
    return std::abs(coins[right].place - coins[left].place);
}

int solve(size_t coin_count, const std::vector<Coin>& coins) {

    std::vector<std::vector<int>> dynamics_left(coin_count, std::vector<int> (coin_count, 0));
    std::vector<std::vector<int>> dynamics_right(coin_count, std::vector<int> (coin_count, 0));

    for (int diff = 1; diff < coin_count; ++diff) {
        for (int index = 0; index + diff < coin_count; ++index) {
            { // left
                int left_value = dynamics_left[index + 1][index + diff] +
                        getDist(coins, index, index + 1);
                int right_value = dynamics_right[index + 1][index + diff] +
                        getDist(coins, index + diff, index);
                if (left_value > coins[index].value) {
                    left_value = 1000001;
                }
                if (right_value > coins[index].value) {
                    right_value = 1000001;
                }

                dynamics_left[index][index + diff] = std::min(left_value, right_value);
                // std::cerr << "left right :: " << left_value << " " << right_value << "\n";
            }
            { // right
                int left_value = dynamics_left[index][index + diff - 1] +
                        getDist(coins, index, index + diff);
                int right_value = dynamics_right[index][index + diff - 1] +
                        getDist(coins, index + diff - 1, index + diff);
                if (left_value > coins[index + diff].value) {
                    left_value = 1000001;
                }
                if (right_value > coins[index + diff].value) {
                    right_value = 1000001;
                }
                dynamics_right[index][index + diff] = std::min(left_value, right_value);
                // std::cerr << "left right :: " << left_value << " " << right_value << "\n";
            }
        }
    }
    return std::min(dynamics_left[0][coin_count - 1], dynamics_right[0][coin_count - 1]);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    size_t coin_count;
    std::vector<Coin> coins;
    // makeRandom(&triangles_count, &triangles);
    readInput(&coin_count, &coins);
    int result = solve(coin_count, coins);

    writeOutput(result);
}
