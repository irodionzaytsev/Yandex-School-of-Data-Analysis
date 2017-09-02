#include <iostream>
#include <algorithm>

int main() {
    int N = 15;
    int M = 10;
    std::cout << N << std::endl;
    for (int i = 0; i < N; ++i) {
        std::cout << std::rand() % M << " ";
    }

}