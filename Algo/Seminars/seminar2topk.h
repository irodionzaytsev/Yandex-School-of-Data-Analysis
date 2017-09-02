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

void readInput(int* N, int* K, int* xm1, int* x0, int* A, int* B, int* C, std::istream &in_stream = std::cin) {
    in_stream >> *N >> *K >> *xm1 >> *x0 >> *A >> *B >> *C;
}

void writeOutput(std::vector<int>& result, std::ostream &out_stream = std::cout) {
    for (int i = 0; i < result.size(); ++i) {
        out_stream << result[i] << " ";
    }

}

class Generator {

private:
    int xm1, x0, A, B, C;
    int iteration = 0;
    int current_x;
    int prev_x;
    int iterations;

public:
    explicit Generator(int N, int ixm1, int ix0, int iA, int iB, int iC):
            iterations(N), xm1(ixm1), x0(ix0), A(iA), B(iB), C(iC) {
        current_x = x0; prev_x = xm1;
    }

    bool hasNext() {
        return iteration + 1 <= iterations;
    }

    int getNext() {
        ++iteration;
        int next = A * prev_x + B * current_x + C;
        if (next < 0) {
            next &= mod;
        }
        prev_x = current_x;
        current_x = next;
        return next;
    }

    void restart() {
        iteration = 0;
        prev_x = xm1;
        current_x = x0;
    }

};

void clear(std::vector<int>* v,int k, int* minimal_element) {
    std::sort((*v).begin(), (*v).end());
    (*v).resize(k);
    (*minimal_element) = -(*v)[k - 1];
}

std::vector<int> solve(const int N, const int K, const int xm1, const int x0, const int A, const int B, const int C) {
    Generator generator(N, xm1, x0, A, B, C);
    std::vector<int> counts(1 << 16, 0);

    for (int i = 0; i < N; ++i) {
        int current_x = generator.getNext();
        ++counts[current_x >> 16];
    }

    int first_half;
    int sum = 0;
    for (int i = counts.size() - 1; i >= 0; --i) {
        first_half = i;
        sum += counts[i];
        if (sum >= K) {
            break;
        }
    }

    std::vector<int> result;
    generator.restart();
    int minimal_element = -1;
    for (int i = 0; i < N; ++i) {
        int current_x = generator.getNext();
        if (current_x >> 16 >= first_half && current_x > minimal_element) {
            result.push_back(-current_x);
        }
        if (result.size() >= 2*K) {
            clear(&result, K, &minimal_element);
        }
    }
    clear(&result, K, &minimal_element);

    for (int i = 0; i < result.size(); ++i) {
        result[i] = -result[i];
    }
    return result;
}


int main() {

    int N, K, xm1, x0, A, B, C;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    readInput(&N, &K, &xm1, &x0, &A, &B, &C);
    std::vector<int> result = solve(N, K, xm1, x0, A, B, C);
    writeOutput(result);
}
