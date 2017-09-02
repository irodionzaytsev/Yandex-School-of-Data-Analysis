#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
#include <list>
#include <ctime>

const long long MOD = 123456789;

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


void readInput(std::vector<long long>* numbers,
               std::istream &in_stream = std::cin) {
    int count_numbers;
    in_stream >> count_numbers;
    numbers->resize(count_numbers);
    for (int i = 0; i < count_numbers; ++i) {
        in_stream >> (*numbers)[i];
    }
}

void writeOutput(long long result,
                 std::ostream &out_stream = std::cout) {
    out_stream << result << "\n";
}


long long solve(const std::vector<long long>& numbers) {
    std::vector<long long> sorted_numbers = numbers;
    int count = sorted_numbers.size();
    std::sort(sorted_numbers.begin(), sorted_numbers.end());
    std::vector<std::vector<long long>> dynamics(count, std::vector<long long>(count, 1));
    for (int len = 1; len < count; ++len) {
        for (int left = 0; left + len < count; ++left) {
            int right = left + len;
            dynamics[left][right] = dynamics[left + 1][right];
            if (sorted_numbers[right] != sorted_numbers[right - 1]) {
                dynamics[left][right] = (dynamics[left][right] + dynamics[left][right - 1]) % MOD;
            }
            for (int inner = left + 1; inner < right; ++inner) {
                if (inner > left && sorted_numbers[inner] == sorted_numbers[inner - 1]) {
                    continue;
                }
                dynamics[left][right] +=
                        (dynamics[left][inner - 1] * dynamics[inner + 1][right]) % MOD;
                dynamics[left][right] %= MOD;
            }
        }
    }
    // printVector(dynamics);
    return dynamics[0][count - 1];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::vector<long long> numbers;
    readInput(&numbers);
    long long result = solve(numbers);
    writeOutput(result);
}
