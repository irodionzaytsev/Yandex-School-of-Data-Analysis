#include <iostream>
#include <utility>
#include <vector>

void read_input(std::vector<int>* v) {
    int element;
    while (std::cin >> element) {
        (*v).push_back(element);
        if (element == 0) {
            break;
        }
    }
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

std::vector<int> reverse(const std::vector<int>* v) {
    std::vector<int> v_copy;
    for (int i = (*v).size() - 1; i >= 0; --i) {
        v_copy.push_back((*v)[i]);
    }
    return v_copy;
}

void write_output(std::vector<int>* v) {
    for (int i = 0; i < (*v).size(); ++i) {
        std::cout << (*v)[i];
        if (i + 1 < (*v).size()) {
            std::cout << std::endl;
        }
    }
}

int main() {
    std::vector<int> v;
    read_input(&v);
    std::vector<int> reverse_copy = reverse(&v);
    write_output(&reverse_copy);
}
