#include <iostream>
#include <utility>
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

void swap(int& a, int& b) {
    int tmp = a;
    a = b;
    b = tmp;
}

void write_output(int *a, int *b) {
    std::cout << *a << " " << *b << std::endl;
}

int main() {
    int a, b;
    read_input(&a, &b);
    swap(a, b);
    write_output(&a, &b);
}
