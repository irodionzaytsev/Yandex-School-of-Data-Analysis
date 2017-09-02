#include <iostream>

void read_input(int *a, int *b) {
    std::cin >> *a >> *b;
}

int gcd(int a, int b) {
    if (a == 0 || b == 0) {
        return a + b;
    }
    if (a > b) {
        return gcd(a - b, b);
    } else {
        return gcd(b - a, a);
    }
}

void write_output(int *g) {
    std::cout << *g << std::endl;
}

int main() {
    int a, b;
    read_input(&a, &b);
    int g = gcd(a, b);
    write_output(&g);
}
