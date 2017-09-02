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

void readInput(std::istream &in_stream = std::cin) {
}

void writeOutput(long double answer, std::ostream &out_stream = std::cout) {
    out_stream.precision(10);
    out_stream << std::fixed;
    out_stream << answer << "\n";
}

class DucheDynamics {
public:
    DucheDynamics(int _count) {
        count = _count;
        dynamics.assign(count + 1, std::vector<long double>(count + 1, 0));

        for (int level = 0; level <= count; ++level) {
            add(0, level, 1.0);
        }

        for (int level = 0; level <= count; ++level) {
            add(1, level, 1.0);
        }

        for (int vertex = 2; vertex <= count; ++vertex) {
            for (int level = 0; level <= count; ++level) {
                for (int before = 0; before < vertex; ++before) {
                    long double poss = get(before, level - 1) * get(vertex - before - 1, level - 1);
                    add(vertex, level, poss / vertex);
                }
            }
        }
    }

    long double get(int vertex, int level) {
        if (level < 0) {
            return 0.0;
        }
        if (level > count) {
            return 1.0;
        }
        // std::cerr << vertex << " " << level << " get\n";
        return dynamics[vertex][level];
    }

    void add(int vertex, int level, long double poss) {
        if (level < 0 || level > count) {
            return;
        }
        // std::cerr << vertex << " " << level << " " << poss << " add\n";
        dynamics[vertex][level] += poss;
    }

private:
    int count;
    std::vector<std::vector<long double>> dynamics;
};

int readInt() {
    int x;
    std::cin >> x;
    return x;
}

long double solve(int vertex, int height) {
    DucheDynamics duche(vertex);
    return duche.get(vertex, height);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    int vertex = readInt();
    int height = readInt();

    long double answer = solve(vertex, height) - solve(vertex, height - 1);
    writeOutput(answer);
}
