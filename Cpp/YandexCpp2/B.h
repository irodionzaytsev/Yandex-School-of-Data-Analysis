#include <iostream>

void read_input(int *a, int *b) {
    std::cin >> *a >> *b;
}

int gcd(const int a, const int b) {
    if (a == 0 || b == 0) {
        return a + b;
    }
    if (a > b) {
        return gcd(a - b, b);
    } else {
        return gcd(b - a, a);
    }
}

void frac_reduction(int* a, int* b) {
    int g = gcd(abs(*a), abs(*b));
    (*a) /= g;
    (*b) /= g;
    if ((*b) < 0) {
        (*a) = -(*a);
        (*b) = -(*b);
    }
}

void write_output(int *a, int *b) {
    std::cout << *a << " " << *b << std::endl;
}

int main() {
    int a, b;
    read_input(&a, &b);
    frac_reduction(&a, &b);
    write_output(&a, &b);
}
