#include <iostream>
#include <string>

int main() {
    std::string s;
    std::cin >> s;
    int occur = 0;
    int place = -1;
    for (int i = 0; i < s.length(); ++i) {
        if (s[i] == 'f') {
            ++occur;
            if (occur <= 2) {
                place = i;
            }
        }
    }

    if (occur >= 2) {
        std::cout << place;
    }
    if (occur == 1) {
        std::cout << -1;
    }
    if (occur == 0) {
        std::cout << -2;
    }

    return 0;
}
