#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
#include <list>
#include <ctime>
#include <stack>

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

};

void readInput(size_t* coin_count,
               std::vector<Coin*>* coins,
               std::istream &in_stream = std::cin) {
    in_stream >> *coin_count;
    int place;
    int value;
    for (int i = 0; i < *coin_count; ++i) {
        in_stream >> place >> value;
        coins->push_back(new Coin(place, value));
    }
}

/*
void makeRandom(size_t* triangles_count,
                std::vector<Triangle*>* triangles,
                std::istream &in_stream = std::cin) {
    int a_side;
    int b_side;
    int c_side;
    *triangles_count = 1000000;
    for (int i = 0; i < *triangles_count; ++i) {
        a_side = rand() % 1000000000 + 1;
        b_side = rand() % 1000000000 + 1;
        c_side = rand() % 1000000000 + 1;
        triangles->push_back(new Triangle(a_side, b_side, c_side));
        triangles->back()->normalize();
    }
}
*/

void writeOutput(int result, std::ostream &out_stream = std::cout) {
    out_stream << result << "\n";
}

bool check(const std::vector<Coin*> coins, const int time) const {
    std::stack<Coin*> coin_stack;
    for (auto& coin_ptr : coins) {
        coin_stack.push(coin_ptr);
    }
    int current_time = time;
    int prev_coord = coin_stack.top()->place;
    while (!coin_stack.empty()) {
        std::stack<Coin*> tmp_stack;
        while(!coin_stack.empty()) {
            Coin* coin_ptr = coin_stack.top();
            coin_stack.pop();
            current_time -= (coin_ptr->place - prev_coord);
            prev_coord = coin_ptr->place;
            if (current_time > coin_ptr->value) {
                tmp_stack.push(coin_ptr);
            }
        }
    }
    coin_stack =
}

int solve(size_t coin_count, const std::vector<Coin*> coins) const {

    int lower_time = 0;
    int upper_time = 1000001;

    std::vector<Coin*> reversed_coins;
    for (int i = coins.size() - 1; i>=0; --i) {
        reversed_coins.push_back(coins[i]);
    }

    while (upper_time > lower_time + 1) {
        int mid_time = (lower_time + upper_time) / 2;
        if (check(coins, mid_time) || check(reversed_coins, mid_time)) {
            upper_time = mid_time;
        } else {
            lower_time = mid_time;
        }
    }

}



int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    size_t coin_count;
    std::vector<Coin*> coins;
    // makeRandom(&triangles_count, &triangles);
    readInput(&coin_count, &coins);
    int result = solve(coin_count, coins);
    for (auto coin :coins) {
        delete coin;
    }
    writeOutput(result);
}
