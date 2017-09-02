#include <iostream>
#include <string>

int main() {
    std::string c1;
    std::string c2;
    std::cin >> c1 >> c2;
    int a1 = c1[0] - 'a';
    int b1 = c1[1] - '0';
    int a2 = c2[0] - 'a';
    int b2 = c2[1] - '0';
    if ((a2 + b2 - a1 - b1) % 2 != 0) {
        std::cout << -1;
        return 0;
    }
    if (a2 + b2 >= a1 + b1 && b2 - a2 >= b1 - a1) {
        int xx1 = a1 + b1;
        int yy1 = a1 - b1;
        int xx2 = a2 + b2;
        int yy2 = a2 - b2;
        std::cout << (abs(xx1 - xx2) + abs(yy1 - yy2))/2;
    } else {
        std::cout << -1;
    }

    return 0;
}
